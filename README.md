# PeaceableQueens
Uses Simulated Annealing to solve the Peaceable Queens problem.

Peaceable Queens Problem: Given a chess board of size nxn, how many pairs of queens of opposite colour can be placed on the board in such a way that no queens of opposite colour can attack one another? There must be an equal number of white and black queens.

Usage: ./peaceablequeens n [m]\
n: the width/height of the nxn board. n must be an integer such that 3 <= n <= 30.\
m: the number of pairs of queens placed on the board. m must be an integer such that 0 <= m < ((n * n) / 2).\
m is optional, and if it is not given, the program will default to the largest known solution for a board of size n.

To compile the code, type "make" into terminal while in this directory.

For more information about this problem, see this YouTube video:
https://www.youtube.com/watch?time_continue=1&v=IN1fPtY9jYg

The currently known maximum solutions are stored in the Online Encyclopedia of Integer Sequences here: https://oeis.org/search?q=A250000&language=english&go=Search
