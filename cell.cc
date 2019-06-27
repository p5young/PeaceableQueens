// File: cell.cc
// Author: Philip Young
// Date: 26 June 2019

#include "cell.h"

// Constructor
Cell::Cell(const int _r, const int _c, const char _display, const int _n) : r(_r), c(_c), n(_n) {
	display = _display;
	occupied = false;
	w_conf = 0;		// number of white queens threatening this square
	b_conf = 0;		// number of black queens threatening this square
} // constructor

// returns the display character for this cell
char Cell::getDisplay() {
	return display;
} // getDisplay

// returns true if this cell is occupied by a queen
bool Cell::Occupied() {
	return occupied;
} // Occupied



// Builds the set of possible moves for a queen on this cell (posb_moves)
void Cell::init(const std::vector< std::vector<Cell*> >& grid) {
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
			// above and to the left
			if (r-i >= 0 && c-i >= 0)
				posb_moves.insert(grid[r-i][c-i]);
			// above and to the right
			if (r-i >= 0 && c+i < n)
				posb_moves.insert(grid[r-i][c+i]);
			// down and to the left
			if (r+i < n && c-i >= 0)
				posb_moves.insert(grid[r+i][c-i]);
			// down and to the right
			if (r+i < n && c+i < n)
				posb_moves.insert(grid[r+i][c+i]);
		} // if
	} // for

} // init


// Adds a queen to this cell
// _display = 'b' for black queen, or 'w' for white queen
void Cell::addQueen(const char _display) {

	// assert preconditions
	assert(!occupied);
	assert(_display == 'w' || _display == 'b');
	
	display = _display;		// set display character
	occupied = true;		// flag this cell as occupied

	// increase conflict counter for all squares I'm threatening
	if (display == 'w') {
		for (Cell* target : posb_moves) {
			target->w_conf += 1;
		} // for
	} else if (display == 'b') {
		for (Cell* target : posb_moves) {
			target->b_conf += 1;
		} // for
	} // if
} // addQueen



// cost() returns the number of conflicts this cell has if it is occupied
// or, if a display character of 'w' or 'b' is passed as an argument, the number
// of conflicts this cell would have if it were occupied.
// cost() can only be called on occupied Cell's
// cost('w') or cost('b') can only be called on unoccupied cells
int Cell::cost(const char _display) {

	// assert preconditions
	if (_display == '+') {
		assert(occupied);
	} else {
		assert(_display == 'w' || _display == 'b');
		assert(!occupied);
	} // if

	// true if we're asking for the conflicts a white queen would have on this square
	bool whitequeen;

	// initialize whitequeen boolean
	if (_display == '+') {			// this cell is occupied
		if (display == 'w') {
			whitequeen = true;
		} else if (display == 'b') {
			whitequeen = false;
		} // if
	} else if (_display == 'w') {
		whitequeen = true;
	} else if (_display == 'b') {
		whitequeen = false;
	} // if

	// return cost value (number of opposing colour queens targeting this square)
	if (whitequeen) {
		return b_conf;
	} else {
		return w_conf;
	} // if
} // cost



// removes the queen from this cell
void Cell::removeQueen() {

	assert(occupied);	// assert precondition

	// reduce conflict counters for every cell I can target
	if (display == 'w') {
		for (Cell* target : posb_moves) {
			target->w_conf -= 1;
		} // for all targets
	} else if (display == 'b') {
		for (Cell* target : posb_moves) {
			target->b_conf -= 1;
		} // for all targets
	} // if white or black queen

	// reset display character
	display = '+';

	// flag as unoccupied
	occupied = false;
} // removeQueen