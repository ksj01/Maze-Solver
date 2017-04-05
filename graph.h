//I Kevin Jacobson have not used any code other than my own(or that in the textbook) for this project. I
//also have not used any function or data structure from the Standard Template Library.I
//understand that any violation of this disclaimer will result in a 0 for the project.

#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

//Class: node
//Creates nodes that contain all the data necessary for our graph to function.
class node {
private:
public:
	node* up;
	node* down;
	node* left;
	node* right;
	int distance;
	node* previous;
	int x;
	int y;
	char value;

	node(char in, int height, int width) {
		value = in;
		up = NULL;
		down = NULL;
		left = NULL;
		right = NULL;
		previous = NULL;
		y = height;
		x = width;
		distance = numeric_limits<int>::max();
	}
	node() {
		value = NULL;
		up = NULL;
		down = NULL;
		left = NULL;
		right = NULL;
		previous = NULL;
		y = NULL;
		x = NULL;
		distance = numeric_limits<int>::max();
	}
};

//Class: graph
//Contains the methods necessary to process an input, turn said input into a 2D array, turn that 2D array into
//a graph, traverse the nodes in the graph to determine the shortest distance from start to end, modify the
//original 2D array to represent the shortest path, and then output that into a complete maze.

class graph {
private:
	char** maze;
	node*** graphMaze;
	int width;
	int height;
	node* start;
	node* end;
	node** queue;
	int total = 0;
public:

	~graph() {
		for (int i = 0; i < width; i++) {
			delete maze[i];
			delete graphMaze[i];
		}
		delete maze;
		delete graphMaze;
	}

	//Method: convert
	//Parameters: none
	//Returns: none
	//Reads in the 2D array that was created and converts any chars that are either ' ', 's', or 'e' into
	//nodes, then adds them to a new 2D array for our graph.
	void convert() {
		total = 0;
		graphMaze = new node**[width];
		for (int i = 0; i < width; i++) {
			graphMaze[i] = new node*[height];
		}
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (maze[j][i] == ' ' || maze[j][i] == 's' || maze[j][i] == 'e') {
					total++;
					node* temp = new node(maze[j][i], j, i);
					if (temp->value == 's') {
						start = temp;
					}
					else  if (temp->value == 'e') {
						end = temp;
					}
					graphMaze[j][i] = temp;
				}
				else {
					graphMaze[j][i] = NULL;
				}
			}
		}
	}

	//Method: dijkstras
	//Parameters: none
	//Returns: none
	//Performs Dijkstra's algorithm on our 2D node array, changing the "previous" and "distance" node values
	//along the way to determine the shortest route. After that is completed, we will change the values of each node
	//along the shortest path from ' ', 's', or 'e' to '.' which will allow us to print out the completed route.
	void dijkstras() {
		//Perform the necessary functions to create a queue out of our graph array.
		queue = new node*[total];
		int count = 0;
		start->distance = 0;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (graphMaze[j][i] != NULL) {
					queue[count] = graphMaze[j][i];
					count++;
				}
			}
		}
		//As long as the queue is not empty, perform Dijkstra's algorithm
		while (count > 0) {
			node* shortest = queue[0];
			int shortestIndex = 0;
			for (int i = 1; i < total; i++) {
				if (queue[i] == NULL) {
					continue;
				}
				else if (shortest == NULL || shortest->distance > queue[i]->distance) {
					shortest = queue[i];
					shortestIndex = i;
				}
			}
			//If there is no path, print out that there is no solution and return.
			if (shortest == NULL || shortest->distance == numeric_limits<int>::max()) {
				cout << "There is no path to the end." << endl;
				return;
			}
			queue[shortestIndex] = NULL;
			count--;
			if (shortest->value == 'e') {
				break;
			}
			//Check the distances of the neighbor nodes and reassign their distance value if necessary.
			//Also reassign their "previous" pointer if necessary.
			int neighborDistance = shortest->distance + 1;
			if (shortest->up != NULL && neighborDistance < shortest->up->distance) {
				shortest->up->distance = neighborDistance;
				shortest->up->previous = shortest;
			}
			if (shortest->down != NULL && neighborDistance < shortest->down->distance) {
				shortest->down->distance = neighborDistance;
				shortest->down->previous = shortest;
			}
			if (shortest->left != NULL && neighborDistance < shortest->left->distance) {
				shortest->left->distance = neighborDistance;
				shortest->left->previous = shortest;
			}
			if (shortest->right != NULL && neighborDistance < shortest->right->distance) {
				shortest->right->distance = neighborDistance;
				shortest->right->previous = shortest;
			}
		}
		//Traverse along the "Previous" pointers, reassigning the values along the way.
		node* current = end;
		maze[end->y][end->x] = '.';
		while (current != start) {
			current = current->previous;
			maze[current->y][current->x] = '.';
		}
		maze[start->y][start->x] = '.';
			delete queue;
	}

	//Method: point
	//Parameters: none
	//Returns: none
	//Traverses the 2D graph array and assigns the directional pointers to any existing neighbors.
	void point() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (graphMaze[j][i] != NULL) {
					if (i > 0) {
						graphMaze[j][i]->up = graphMaze[j][i - 1];
					}
					if (i < height -1) {
						graphMaze[j][i]->down = graphMaze[j][i + 1];
					}
					if (j > 0) {
						graphMaze[j][i]->left = graphMaze[j - 1][i];
					}
					if (j < width -1) {
						graphMaze[j][i]->right = graphMaze[j + 1][i];
					}
				}
			}
		}
	}

	//Method: process_file
	//Parameters: istream&
	//Returns: bool
	//Parses whatever input it recieves into a 2D array.
	bool process_file(istream& input) {
		string dimensions;
		getline(input, dimensions);
		stringstream dimLine(dimensions);
		dimLine >> width;
		dimLine >> height;
		maze = new char*[width];
		for (int i = 0; i < width; i++) {
			maze[i] = new char[height];
		}
		for (int i = 0; i < height; i++) {
			string line;
			getline(input, line);
			for (int j = 0; j < width; j++) {
				maze[j][i] = line[j];
			}
		}
		convert();
		point();
		return true;
	}

	//Method: print
	//Parameters: ostream&
	//Returns: void
	//Prints out the completed maze to the passed output.
	void print(ostream& output) {
		output << width << " " << height << endl;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				output << maze[j][i];
			}
			output << endl;
		}
	}
};

