# Makefile
#
# Any files which are required for compilation must have their .o name put in the OBJECTS list
# eg. cell.h and cell.cc require cell.o being added
#
# supports:
# make
# make clean
# make run
# make peaceablequeens

########## Variables ##########

CXX = g++										# compiler
CXXFLAGS = -g -Wall -Werror -std=c++11 -MMD		# compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

EXEC = peaceablequeens							# executable name
OBJECTS = peaceablequeens.o board.o cell.o		# object files
DEPENDS = ${OBJECTS:.o=.d}						# substitute ".o" with ".d"

########## Targets ##########

.PHONY : clean							# not file names

${EXEC} : ${OBJECTS}					# link step
	${CXX}  $^ -o $@					# additional object files before $^

${OBJECTS} : ${MAKEFILE_NAME}		# OPTIONAL : changes to this file => recompile

# make implicitly generates rules to compile C++ files that generate .o files

-include ${DEPENDS}						# include *.d files containing program dependences

clean :									# remove files that can be regenerated
	rm -f ${DEPENDS} ${OBJECTS} ${EXEC}

run :
	@${MAKE} ${EXEC}	> /dev/null 	# build the program
	@./${EXEC} 4						# run it - default board size is the minimum (4x4)

