#include "board.h"


// constructor
// builds _n * _n board and randomly places a white queen and a black queen
Board::Board(int _n) {
	// initialize n (board width/height)
	n = _n;

	// initialize number of conflicts to 0 since board is empty
	conflicts = 0;

	// number of queens on the board
	num_queens = 0;

	// set grid size to n * n
	grid.resize(n);
	for (int i = 0 ; i < n ; ++i)
		grid[i].resize(n);

	// initialize every cell
	for (int i = 0 ; i < n ; ++i) {
		for (int j = 0 ; j < n ; ++j) {
			grid[i][j] = new Cell(i, j, '+', n);
		}
	}

	// for each cell, add its possible moves and neighbours
	for (int i = 0 ; i < n ; ++i) {
		for (int j = 0 ; j < n ; ++j) {
			grid[i][j]->init(grid);
			no_queens.insert(grid[i][j]);	// add to set of empty cells
		}
	}

	// Initialize random number generator
	srand(time(0));
}



// destructor
Board::~Board() {
	// delete cells
	for (int i = 0 ; i < n ; ++i) {
		for (int j = 0 ; j < n ; ++j) {
			delete grid[i][j];
		}
	}	 
}



// returns a random element from the unordered_set or vector
template <typename I>
I Board::randomElement(I begin, I end) {
	const unsigned long n = std::distance(begin, end);
	const unsigned long divisor = (RAND_MAX) / n;

	unsigned long k;
	// NOTE: This loop almost always only executes once
	do {
		k = std::rand() / divisor;
	} while (k >= n);

	std::advance(begin, k);
	return begin;
}



void Board::moveQueen(Cell* oldCell, Cell* nextCell) {
	if (oldCell == nextCell)
		return;

	char display = oldCell->display;
	if (display == 'w') {
		conflicts -= oldCell->b_conf;
		conflicts += nextCell->b_conf;
	} else {
		conflicts -= oldCell->w_conf;
		conflicts += nextCell->w_conf;
	}

	oldCell->removeQueen();
	nextCell->addQueen(display);

	// move oldCell to no_queens
	queens.erase(oldCell);
	no_queens.insert(oldCell);

	// add nextCell to queens
	no_queens.erase(nextCell);
	queens.insert(nextCell);
}



// returns the number of pairs of opposite colour queens which can attack one another
int Board::getConflicts() {
	return conflicts;
}

// Adds another white and black queen pair to the board in a random location
void Board::addPair() {

	assert(no_queens.size() >= 2);

	// ADD WHITE QUEEN //

	// get white queen from set of empty cells
	Cell* white_queen = *randomElement(no_queens.begin(), no_queens.end());

	// double check the cell is empty
	assert(!white_queen->occupied);

	// move cell pointer from no_queens to queens
	no_queens.erase(white_queen);
	queens.insert(white_queen);

	// add the queen to the cell
	white_queen->addQueen('w');

	// increase conflict counter if necessary
	conflicts += white_queen->b_conf;

	// ADD BLACK QUEEN //

	// get black queen from set of empty cells
	Cell* black_queen = *randomElement(no_queens.begin(), no_queens.end());

	// double check the cell is empty
	assert(!black_queen->occupied);

	// move cell pointer from no_queens to queens
	no_queens.erase(black_queen);
	queens.insert(black_queen);

	// add the queen to the cell
	black_queen->addQueen('b');

	// increase conflict counter if necessary
	conflicts += black_queen->w_conf;

	// increase counter
	++num_queens;
}



void Board::simulatedAnnealing() {
	int x = 5000000;				// x is the number of times T will be decrimented (represents x axis of T  = f(x))
	float T = 10.0;					// initialize T to starting temperature
	float k = x / log(T + 1);		// allows you to change T at will and logarithmic annealing schedule will be maintained over domain x

	// Since the algorithm takes a while, notify the user whenever 1/10th of the algorithm is complete
	int notify_user = x / 10;
	const int notify_reset_val = notify_user;
	int notifies_remaining = 10;

	while (true) {

		// Set T
		T = exp(x / k) - 1;			// - 1 so when x = 0, T = 0
				
		// EXIT LOOP WHEN T <= 0 or a solution is found
		if (conflicts == 0 || T <= 0)
			break;

		// select a random queen
		Cell* oldCell = *randomElement(queens.begin(), queens.end());

		// find a random neighbour of selected queen
		Cell* nextCell = *randomElement(no_queens.begin(), no_queens.end());

		// find difference in cost (lower is better)
		int curr_conflicts = oldCell->cost();
		int new_conflicts = nextCell->cost(oldCell->display);
		int cost_diff = curr_conflicts - new_conflicts;

		// move the queen if cost is better
		// if cost is worse, move with probability determined by cost difference and temperature
		if (cost_diff > 0) {
			moveQueen(oldCell, nextCell);
		} else {
			// make a random float from 0.0 to 1.0 inclusive
			float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

			if ( r <= exp(cost_diff / T) ) {
				moveQueen(oldCell, nextCell);
			}
		}

		// decriment x
		--x;

		--notify_user;
		if (notify_user == 0) {
			// print and decriment counter
			std::cout << notifies_remaining-- << std::endl;
			// reset counter
			notify_user = notify_reset_val;
		}
	}
}



// print the board
void Board::print() {

	std::cout << "Pairs of queens on board: " << num_queens << std::endl;
	// print header - just a row of '-'s
	std::cout << std::string(n, '-') << std::endl;

	// print board
	for (int i = 0 ; i < n ; ++i) {
		for (int j = 0 ; j < n ; ++j) {
			std::cout << grid[i][j]->getDisplay();
		}
		std::cout << std::endl;
	}

	// print footer
	std::cout << std::string(n, '-') << std::endl;
}