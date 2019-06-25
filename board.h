#ifndef BOARD
#define BOARD

#include <iostream>
#include <vector>	// For grid
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()
#include <cmath>	// For log() and exp()
#include <unordered_set>
#include <iterator>

#include "cell.h"

class Board {

	// matrix width/height
	int n;

	// number of queens under attack
	int conflicts;

	// number of queens placed
	int num_queens;

	// hash table of cells where queens are
	std::unordered_set<Cell*> queens;

	// hash table of empty cells (no queen)
	std::unordered_set<Cell*> no_queens; 

	// the matrix of cells forming the board
	std::vector< std::vector<Cell*> > grid;

	// returns a random element from an unordered_set or vector
	template <typename I>
	static I randomElement(I begin, I end);

	// moves a queen from oldCell to newCell
	void moveQueen(Cell* oldCell, Cell* newCell);


	public:
		// constructor
		Board(int _n);

		// destructor
		~Board();

		// returns the number of pairs of opposite colour queens which can attack one another
		int getConflicts();

		// adds a white and a black queen to the board in random locations
		void addPair();

		// runs the simulated annealing algorithm for a given number of queens
		void simulatedAnnealing();

		// displays the board
		void print();
};

#endif