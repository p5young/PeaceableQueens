// File: cell.cc
// Author: Philip Young
// Date: 26 June 2019

#include "cell.h"

// Constructor
Cell::Cell(int _r, int _c, char _display, int _n) {
	r = _r;
	c = _c;
	display = _display;
	n = _n;
	occupied = false;
	w_conf = 0;		// number of white queens threatening this square
	b_conf = 0;		// number of black queens threatening this square
}

// returns the display character for this cell
char Cell::getDisplay() {
	return display;
}

// returns true if this cell is occupied by a queen
bool Cell::Occupied() {
	return occupied;
}



// Builds the set of possible moves for a queen on this cell (posb_moves)
// And builds the set of neighbours for a queen on this cell (neighbours)
void Cell::init(std::vector< std::vector<Cell*> >& grid) {
	// add posb_moves cells - ie. they're in my row, column, or diagonal to me
	for (int i = 0 ; i < n ; ++i) {
		// add column to possible moves (posb_moves)
		if (i != r)
			posb_moves.insert(grid[i][c]);
		// add row to possible moves (posb_moves)
		if (i != c)
			posb_moves.insert(grid[r][i]);
		// add diagonal possible moves distance i from myself
		if (i > 0) {
			if (r-i >= 0 && c-i >= 0)
				posb_moves.insert(grid[r-i][c-i]);
			if (r-i >= 0 && c+i < n)
				posb_moves.insert(grid[r-i][c+i]);
			if (r+i < n && c-i >= 0)
				posb_moves.insert(grid[r+i][c-i]);
			if (r+i < n && c+i < n)
				posb_moves.insert(grid[r+i][c+i]);
		}
	}

	// add every square on the board except this one as neighbours
	for (int i = 0 ; i < n ; ++i) {
		for (int j = 0 ; j < n ; ++j) {
			// make sure either row is different or column is different so I don't add myself
			if (i != r || j != c) {
				neighbours.insert(grid[i][j]);
			}
		}
	}
}


// Adds a queen to this cell
// _display = 'b' for black queen, or 'w' for white queen
void Cell::addQueen(char _display) {

	assert(!occupied);
	assert(_display == 'w' || _display == 'b');
	
	display = _display;
	occupied = true;

	if (display == 'w') {
		for (Cell* target : posb_moves) {
			target->w_conf += 1;
		}
	} else if (display == 'b') {
		for (Cell* target : posb_moves) {
			target->b_conf += 1;
		}
	}
}



// cost() returns the number of conflicts this cell has if it is occupied
// or, if a display character of 'w' or 'b' is passed as an argument, the number
// of conflicts this cell would have if it were occupied.
// cost() can only be called on occupied Cell's
// cost('w') or cost('b') can only be called on unoccupied cells
int Cell::cost(char _display) {

	if (_display == '+') {
		assert(occupied);
	} else {
		assert(_display == 'w' || _display == 'b');
		assert(!occupied);
	}

	// true if we're asking for the conflicts a white queen would have on this square
	bool whitequeen;

	// initialize whitequeen boolean
	if (_display == '+') {			// this cell is occupied
		if (display == 'w') {
			whitequeen = true;
		} else if (display == 'b') {
			whitequeen = false;
		}
	} else if (_display == 'w') {
		whitequeen = true;
	} else if (_display == 'b') {
		whitequeen = false;
	}

	// return cost value (number of opposing colour queens targeting this square)
	if (whitequeen) {
		return b_conf;
	} else {
		return w_conf;
	}
}



// removes the queen from this cell
void Cell::removeQueen() {

	assert(occupied);

	// reduce conflict counters for every cell I can target
	if (display == 'w') {
		for (Cell* target : posb_moves) {
			target->w_conf -= 1;
		}
	} else if (display == 'b') {
		for (Cell* target : posb_moves) {
			target->b_conf -= 1;
		}
	}

	// reset display character
	display = '+';

	// flag as unoccupied
	occupied = false;
}