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
	// w_conf: number of white queens threatening this square
	// b_conf: number of black queens threatening this square
	int w_conf, b_conf;

	// Possible Moves - cells which are diagonal or in the same row or column
	std::unordered_set<Cell*> posb_moves;

	// Neighbours - When the queen in this cell is looking for a new home (FindNext()) this group is searched
	// Use smaller or larger neighbourhoods to affect algorithm speed and output
	std::unordered_set<Cell*> neighbours;

	public:
		// constructor
		Cell(int _r, int _c, char _display, int _n);

		// creates posb_moves and neighbourhood hash tables
		void init(std::vector< std::vector<Cell*> >& grid);

		// returns the display character for this cell
		char getDisplay();

		// returns the number of conflicts this cell has
		// or, if a display character of 'w' or 'b' is passed as an argument, the number of conflicts this cell would have
		int cost(char _display = '+');

		void addQueen(char _display);

		void removeQueen();

		// returns a random element from an unordered_set or vector
		template <typename I>
		static I randomElement(I begin, I end);

		Cell* findNext();
};
#endif