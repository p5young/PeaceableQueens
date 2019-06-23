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
		}
	}


	// Initialize random number generator
	srand(time(0));

	// places the first 2 queens on the board
	addPair();

	// print initial message to user
	std::cout << "Board of size " << n << "x" << n << " created.\n" << "Working..." << std::endl;

	// display starting board
	//std::cout << "Starting Board" << std::endl;
	//print();

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

// Adds another white and black queen pair to the board in a random location
void Board::addPair() {

	// row and column of queens to be placed
	int r, c;
	// colour of queen (changed to 'b' after 'w' is placed)
	char colour = 'w';

	// loop twice, once for white queen, once for black
	for (int i = 0 ; i < 2 ; ++i) {
		// find coordinates which are free
		do {
			r = rand() % n;
			c = rand() % n;
		} while (grid[r][c]->occupied);

		// place the queen
		addQueen(r, c, colour);
		colour = 'b';
	}

	// increase counter
	++num_queens;
}

void Board::addQueen(int r, int c, char colour) {
	grid[r][c]->addQueen(colour);
	queens.insert(grid[r][c]);

	if (colour == 'w') {
		conflicts += grid[r][c]->b_conf;
	} else {
		conflicts += grid[r][c]->w_conf;
	}
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
	queens.erase(oldCell);
	queens.insert(nextCell);
}

// print the board
void Board::print() {

	std::cout << "number of queens placed: " << num_queens << std::endl;
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

void Board::simulatedAnnealing() {
	int x = 5000000;				// x is the number of times T will be decrimented (represents x axis of T  = f(x))
	float T = 10.0;					// initialize T to starting temperature
	float k = x / log(T + 1);		// determine constant k based on starting temperature T and domain of x

	while (true) {

		// Set T
		T = exp(x / k) - 1;			// - 1 so when x = 0, T = 0
				
		// EXIT LOOP WHEN T <= 0 or a solution is found
		if (conflicts == 0 || T <= 0)
			break;

		// select a random queen
		Cell* oldCell = *randomElement(queens.begin(), queens.end());

		// find a random neighbour of selected queen
		Cell* nextCell;
		do {
			nextCell = *randomElement(oldCell->neighbours.begin(), oldCell->neighbours.end());
		} while(nextCell->occupied);

		// find difference in cost (lower is better)
		int curr_conflicts = oldCell->cost();
		int new_conflicts = nextCell->cost(oldCell->display);
		int cost_diff = new_conflicts - curr_conflicts;

		// move the queen if cost is better
		// if cost is worse, move with probability determined by cost difference and temperature
		if (cost_diff < 0) {
			moveQueen(oldCell, nextCell);
		} else {
			// make a random float from 0.0 to 1.0 inclusive
			float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

			if (exp(cost_diff / T) <= r) {
				moveQueen(oldCell, nextCell);
			}
		}

		// decriment x
		--x;
	}
}

// runs Simulated Annealing algorithm
void Board::run() {

	// continue adding pairs of queens until the simulatedAnnealing() routine fails to find a solution
	while (true) {
		simulatedAnnealing();

		if (conflicts == 0) {
			std::cout << "Solution found - current pairs:" << num_queens << std::endl;
			print();
			addPair();
		} else {
			// no solution found, exit
			break;
		}

	}
}