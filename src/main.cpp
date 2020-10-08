/* A maze generator and solver*/
#include "maze.h"

int main() {

	// define dimensions
	int height = 20;
	int width = 20;

	// create maze object
	Maze maze(height, width);

	// Display maze dimensions and initialise maze
	maze.displayMazeDimensions();
	maze.initialiseMaze();

	// Generate maze (using depth-first search with backtracker algorithm) and display it
	maze.generateMaze();
	maze.displayMaze();

	// Solve maze (using A* algorithm) and display it
	maze.solveMaze();
	maze.displayMaze(); 

	// Save solution to a txt file
	maze.saveSolutionToTxtFile();

	return 0;

}