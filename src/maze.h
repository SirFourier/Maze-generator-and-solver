#pragma once
#include "cell.h"
#include <vector>
#include <string>

class Maze {
private:
	// number of cells described with width and height
	int height;
	int width;

	// 2d maze vector
	std::vector<std::vector<Cell>> maze;

	// create vector of strings to store directions of solution
	std::vector<std::string> solution;

public:
	// constructor and methods
	Maze(const int inputHeight = 10, const int inputWidth = 10);
	void displayMazeDimensions() const;
	void initialiseMaze();
	void displayMaze() const;
	void generateMaze();
	void solveMaze();
	void saveSolutionToTxtFile();

};

