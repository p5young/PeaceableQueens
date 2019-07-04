// File: peaceablequeens.cc
// Author: Philip Young
// Date: 26 June 2019

/* Desc:
./peaceablequeens n [m]
int n: grid width/height where 3 <= n <= 30
int m: number of pairs of queens placed on board where 0 <= m < ((n * n) / 2) (optional)

if m is specified, the program will try to find an arrangement where no queens of opposing colour can attack one another.
if m is not specified, the program will default to the currently known maximum and place that many pairs.
*/

#include "board.h"

// Displays the proper usage of the program and exits
void usage( char *argv[] ) {
    std::cerr << "Usage: " << argv[0] << " n [m]" << std::endl
    << "n: is the width/height of the grid" << std::endl
    << "3 <= n <= 30" << std::endl
    << "m: is the number of pairs of queens (optional)" << std::endl
    << "0 <= m < (n * n) / 2" << std::endl;
    exit( EXIT_FAILURE );               // TERMINATE
} // usage

// Returns true if a string is composed entirely of digits, false otherwise
// Used for checking command line arguments
bool is_number(const std::string& s) {
	if (s.empty()) return false;
    for ( unsigned int i = 0 ; i < s.length() ; ++i ) {
    	if (!isdigit(s[i])) {
    		return false;
    	} // if
    } // for
    return true;
} // is_number

// Processes command line argument, initializes the board, and calls simulatedAnnealing() on the board.
int main (int argc, char *argv[] ) {

	// grid width/height
	int n = 0;
	// number of starting queens
	int m = 0;

	// Ainsley's values for n = 1 through 30
	// Source: https://oeis.org/search?q=A250000&language=english&go=Search
	int max_found[] = {0, 0, 1, 2, 4, 5, 7, 9, 12, 14, 17, 21, 24, 28, 32, 37, 42, 47, 52, 58, 64, 70, 77, 84, 91, 98, 105, 114, 122, 131};

	// check for acceptable number of arguments
	if (argc < 2) {
		std::cerr << "No arguments given. Minimum number of arguments is 1" << std::endl;
		usage(argv);
	}
	if (argc > 3) {
		std::cerr << "Too many arguments given. Maximum number of arguments is 2" << std::endl;
		usage(argv);
	}

	// check if first argument is a positive number
	if (!is_number(argv[1])) {
		std::cerr << "n must be a positive number" << std::endl;
		usage(argv);
	}

	// initialize n
	n = std::atoi(argv[1]);

	// check if n is in the proper range
	if (n < 3 || 30 < n) {
		std::cerr << "n must be in the range 3 <= n <= 30" << std::endl;
		usage(argv);
	}

	// process second argument, if present
	if (argc == 3) {

		// check if second argument is a positive number
		if (!is_number(argv[2])) {
			std::cerr << "m must be an integer greater than or equal to 0. You gave " << argv[2] << std::endl;
			usage(argv);
		}

		// initialize m
		m = std::atoi(argv[2]);

		// check if there are too many queens for the board
		if (m * 2 >= n * n) {
			std::cerr << m << " pairs of queens will not fit on a board of size " << n << std::endl;
			usage(argv);
		}
	} else {
		// if no argument is given, default to the currently known maximum
		m = max_found[n-1];
	}

	// print information to user
	std::cout << "n: " << n << std::endl
	<< "m: " << m << std::endl
	<< "Note: the largest known solution found for a board of size " << n << " is " << max_found[n-1] << std::endl
	<< "Starting..." << std::endl;

	// Create the board
	Board board(n);

	// place m pairs of queens in random locations
	for (int i = 0 ; i < m ; ++i) {
		board.addPair();
	}

	// run simulated annealing algorithm
	board.simulatedAnnealing();

	// print board whether a solution is found or not
	board.print();

	if (board.getConflicts() > 0) {
		std::cout << "Sorry, I couldn't find a solution" << std::endl;
	} else {
		std::cout << "Solution found" << std::endl;
	}
	std::cout << "Number of conflicts: " << board.getConflicts() << std::endl;

} // main
