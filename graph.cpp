//I Kevin Jacobson have not used any code other than my own(or that in the textbook) for this project. I
//also have not used any function or data structure from the Standard Template Library.I
//understand that any violation of this disclaimer will result in a 0 for the project.

#include "stdafx.h"
#include "graph.h"
#include <cassert>

//Method: completeMaze()
//Parameters: none
//Returns: none
//Unit Test Case. Reads in a simple maze and verifies it outputs the correct completed maze.
void completeMaze() {
	string maze = "7 4\n#######\n#   #s#\n#e#   #\n#######\n";
	string expected = "7 4\n#######\n#...#.#\n#.#...#\n#######\n";
	stringstream input(maze);
	graph graphy;
	graphy.process_file(input);
	graphy.dijkstras();
	stringstream output;
	graphy.print(output);
	cout << output.str();
	assert(output.str() == expected);
}

//Method: incompleteMaze()
//Parameters: none
//Returns: none
//Unit Test Case. Reads in a maze that cannot be completed. Verifies that it outputs a maze that is not complete.
void incompleteMaze() {
	string maze = "7 4\n#######\n#   #s#\n#e##  #\n#######\n";
	string expected = "7 4\n#######\n#   #s#\n#e##  #\n#######\n";
	stringstream input(maze);
	graph graphy;
	graphy.process_file(input);
	graphy.dijkstras();
	stringstream output;
	graphy.print(output);
	cout << output.str();
	assert(output.str() == expected);
}

//Method: findShortest()
//Parameters: none
//Returns: none
//Unit Test Case. Reads in a map that has 2 valid solutions, but only the shortest is filled in.
void findShortest() {
	string maze = "7 5\n#######\n#   #s#\n#e# # #\n#     #\n#######\n";
	string expected = "7 5\n#######\n#   #.#\n#.# #.#\n#.....#\n#######\n";
	stringstream input(maze);
	graph graphy;
	graphy.process_file(input);
	graphy.dijkstras();
	stringstream output;
	graphy.print(output);
	cout << output.str();
	assert(output.str() == expected);
}

//Method: noWalls()
//Parameters: none
//Returns: none
//Unit Test Case. Reads in a maze with no walls and verifies a correct output
void noWalls() {
	string maze = "1 2\ns\ne\n";
	string expected = "1 2\n.\n.\n";
	stringstream input(maze);
	graph graphy;
	graphy.process_file(input);
	graphy.dijkstras();
	stringstream output;
	graphy.print(output);
	cout << output.str();
	assert(output.str() == expected);
}

//Method: runUnitTests()
//Parameters: none
//Returns: none
//Only purpose is to run all of my unit test cases.
void runUnitTests() {
	completeMaze();
	incompleteMaze();
	findShortest();
	noWalls();
}

int main(int argc, char **argv) {
	if (argc != 3) {
		cerr << "Usage: " << argv[0] << " FILE" << endl;
		return EXIT_FAILURE;
	}
	ifstream filein;
	filein.open(argv[1]);
	if (!filein.is_open()) {
		return false;
	}
	//runUnitTests();
	graph graphy;
	graphy.process_file(filein);
	filein.close();
	graphy.dijkstras();
	ofstream output;
	output.open(argv[2]);
	graphy.print(output);
}
