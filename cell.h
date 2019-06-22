#ifndef CELL
#define CELL

#include <unordered_set>
#include <vector>
#include <iostream>
#include <assert.h>

class Cell {

	friend class Board;

	// Coordinates - row and column
	int r, c;

	// size of grid
	int n;

	// Display character when board is printed
	// + : empty
	// b : black queen
	// w : white queen
	char display;

	// True if there is a queen here
	bool occupied;

	// number of queens threatening this square
	int w_conf, b_conf;

	// Neighbours - cells which are diagonal or in the same row or column
	std::unordered_set<Cell*> neighbours;

	public:
		// constructors
		Cell();
		Cell(int _r, int _c, char _display, int _n);

		// creates neighbourhoods
		void setNeighbours(std::vector< std::vector<Cell*> >& grid);

		// returns the display character for this cell
		char getDisplay();

		void addQueen(char _display);

		void removeQueen();

		// returns a random element from an unordered_set or vector
		template <typename I>
		static I randomElement(I begin, I end);

		Cell* findNext();
};
#endif