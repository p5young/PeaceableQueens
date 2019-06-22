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

void Cell::setNeighbours(std::vector< std::vector<Cell*> >& grid) {
	// make sure cells have been properly initialized
	if (r == -1) {
		std::cerr << "Cell is not initialized yet" << std::endl;
		return;
	}

	// add neighbouring cells - ie. they're in my row, column, or diagonal to me
	for (int i = 0 ; i < n ; ++i) {
		// add column to neighbours
		if (i != r)
			neighbours.insert(grid[i][c]);
		// add row to neighbours
		if (i != c)
			neighbours.insert(grid[r][i]);
		// add diagonal neighbours distance i from myself
		if (i > 0) {
			if (r-i >= 0 && c-i >= 0)
				neighbours.insert(grid[r-i][c-i]);
			if (r-i >= 0 && c+i < n)
				neighbours.insert(grid[r-i][c+i]);
			if (r+i < n && c-i >= 0)
				neighbours.insert(grid[r+i][c-i]);
			if (r+i < n && c+i < n)
				neighbours.insert(grid[r+i][c+i]);
		}
	}
}

void Cell::addQueen(char _display) {

	assert(!occupied);
	
	display = _display;
	occupied = true;

	if (display == 'w') {
		for (Cell* neighbour : neighbours) {
			neighbour->w_conf += 1;
		}
	} else if (display == 'b') {
		for (Cell* neighbour : neighbours) {
			neighbour->b_conf += 1;
		}
	}

}

// removes the queen from this cell
void Cell::removeQueen() {

	assert(occupied);

	if (display == 'w') {
		for (Cell* neighbour : neighbours) {
			neighbour->w_conf -= 1;
		}
	} else if (display == 'b') {
		for (Cell* neighbour : neighbours) {
			neighbour->b_conf -= 1;
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

// returns the neighbour with the lowest cost - breaks ties randomly
Cell* Cell::findNext() {

	assert(occupied);

	// initialize min_cost as the cost of this cell
	int min_cost;
	if (display == 'w') {
		min_cost = b_conf;
	} else if (display == 'b') {
		min_cost = w_conf;
	}

	// hash-table of neighbours with costs equal to or below this one
	std::unordered_set<Cell *> options;

	// search neighbourhood for next move
	for (Cell* neighbour : neighbours) {

		// skip occupied cells since we can't move there
		if (neighbour->occupied)
			continue;
		
		// determine cost of neighbour
		int cost;
		if (display == 'w') {
			cost = neighbour->b_conf;
		} else if (display == 'b') {
			cost = neighbour->w_conf;
		}

		// add to list of options if it ties min cost
		// start new list and add it if it beats min cost
		if (cost < min_cost) {
			min_cost = cost;
			options.clear();
			options.insert(neighbour);
		} else if (cost == min_cost) {
			options.insert(neighbour);
		}
	}

	// return a random cell from options
	if (options.size() > 0) {
		return *randomElement(options.begin(), options.end());
	} else {
		return this;
	}
}