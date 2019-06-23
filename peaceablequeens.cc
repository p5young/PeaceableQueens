// File: peaceablequeens.cc
// Author: Philip Young
// Date: 13 June 2019
// Desc: takes in a single command line argument, n, where 3 <= n <= 30, and attempts to fit as many pairs of black
// 		and white queens as possible on an n*n board using Local Search and Hillclimbing (will possibly use
//		simulated annealing in the future)


#include <iostream>
#include <cstdlib>

#include "board.h"

// Displays the proper usage of the program and exits
void usage( char *argv[] ) {
    std::cerr << "Usage: " << argv[0] << " n" << std::endl
    << "where n is the width/height of the grid" << std::endl
    << "3 <= n <= 30" << std::endl;
    exit( EXIT_FAILURE );               // TERMINATE
} // usage

// Returns true if a string is composed entirely of digits, false otherwise
// Used for checking command line argument
bool is_number(const std::string& s) {
	if (s.empty()) return false;
    bool isNumber = true;
    for ( unsigned int i = 0 ; i < s.length() ; ++i ) {
    	if (!isdigit(s[i])) {
    		isNumber = false;
    	}
    }
    return isNumber;
}

// Processes command line argument, initializes the board, and calls run() on the board.
int main (int argc, char *argv[] ) {

	// grid width/height
	int n = 0;

	// process command line arguments
	switch (argc) {
		case 2:
			// 1 argument - proper number
			// check if argument is a positive number
			if (is_number(argv[1])) {
				// check if argument is in the correct range
				n = std::atoi(argv[1]);		// also initializes n which is used below
				if (3 <= n && n <= 30) {
					// argument is acceptable, break and continue
					break;
				}
			}
			// error found with input
			std::cerr << "n must be a positive number between 4 and 30 inclusive" << std::endl;
			usage(argv);
		case 1:
			// error: too few arguments (none)
			std::cerr << "No arguments found" << std::endl;
			usage(argv);
		default:
			// error: too many arguments
			std::cerr << "Too many arguments" << std::endl;
			usage(argv);
	}

	// create the board - n is initialized when command line arguments are processed
	Board* board = new Board(n);

	// runs hillclimbing
	board->run();

	// delete the board
	delete board;

} // main
