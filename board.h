#ifndef BOARD
#define BOARD

#include <iostream>
#include <vector>	// For grid
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()
#include <unordered_set>
#include <iterator>

#include "cell.h"

class Board {

	// matrix width/height
	int n;

	// number of queens under attack x2 (should always be even)
	int conflicts;

	// number of queens placed with no conflicts
	int max_solution;

	// vector of cells where queens are
	std::unordered_set<Cell*> queens;

	// the matrix of cells forming the board
	std::vector< std::vector<Cell*> > grid;

	// returns a random element from an unordered_set or vector
	template <typename I>
	static I randomElement(I begin, I end);

	// adds a white and a black queen to the board in random locations
	void addPair();

	// adds a queen to a given cell
	void addQueen(int r, int c, char colour);

	// moves a queen from oldCell to newCell
	void moveQueen(Cell* oldCell, Cell* newCell);

	public:
		// constructor
		Board(int _n);

		// destructor
		~Board();

		// displays the board
		void print();

		// runs the hillclimbing algorithm
		void run();
};

#endif