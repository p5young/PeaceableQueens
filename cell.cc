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



void Cell::addQueen(char _display) {

	assert(!occupied);
	
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



// returns the number of conflicts this cell has
// or, if a display character of 'w' or 'b' is passed as an argument, the number of conflicts this cell would have
int Cell::cost(char _display) {

	// true if we're asking for the conflicts a white queen would have on this square
	bool whitequeen;
	if (_display == '+') {
		assert(occupied);
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

	if (whitequeen) {
		return b_conf;
	} else {
		return w_conf;
	}
}



// removes the queen from this cell
void Cell::removeQueen() {

	assert(occupied);

	if (display == 'w') {
		for (Cell* target : posb_moves) {
			target->w_conf -= 1;
		}
	} else if (display == 'b') {
		for (Cell* target : posb_moves) {
			target->b_conf -= 1;
		}
	}

	display = '+';
	occupied = false;
}



// returns a random element from the unordered_set or vector
template <typename I>
I Cell::randomElement(I begin, I end) {
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
