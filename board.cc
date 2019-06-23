#include "board.h"


// constructor
// builds _n * _n board and randomly places a white queen and a black queen
Board::Board(int _n) {
	// initialize n (board width/height)
	n = _n;

	// initialize number of conflicts to 0 since board is empty
	conflicts = 0;

	// number of queens on the board
	max_solution = 0;

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

	std::cout << "number of queens placed: " << max_solution << std::endl;
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

// 
void Board::run() {
	for (int i = 0 ; i < 1000000 ; ++i) {
		// select a random queen
		Cell* oldCell = *randomElement(queens.begin(), queens.end());

		// find a neighbour with minimum cost (break ties randomly)
		Cell* nextCell = oldCell->findNext();

		// move the queen
		moveQueen(oldCell, nextCell);

		//std::cout << "Conflicts: " << conflicts << std::endl;
		//print();

		if (conflicts == 0) {
			max_solution += 1;
			std::cout << "Solution found - current pairs:" << max_solution << std::endl;
			print();
			addPair();

			// reset counter - each solution gets 1000000 moves worth of trying
			i = 0;
		}
	}
}