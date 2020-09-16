// cpp file for maze class in maze.h
#include <iostream>
#include "maze.h"
#include "cell.h"
#include <ctime>
#include <fstream>

// initialising dimensions
Maze::Maze(const int inputHeight, const int inputWidth)
	: height{ inputHeight }, width{ inputWidth } {
	std::cout << "Dimensions have been initialised\n";
}

// display maze dimensions
void Maze::displayMazeDimensions() const {
	std::cout << "The width of the maze is " << width << std::endl;
	std::cout << "The height of the maze is " << height << std::endl;
}

// create empty maze full of walls
void Maze::initialiseMaze() {
	
	for (int i = 0; i < height; i++) {
		std::vector<Cell> mazeRow;
		for (int j = 0; j < width; j++) {
			Cell cell;
			cell.row = i;
			cell.col = j;
			mazeRow.push_back(cell);
		}
		maze.push_back(mazeRow);
	}
	std::cout << "Maze has been intialised\n";
}

// display maze using raster scan (if solution found it will also display that)
void Maze::displayMaze() const {

	for (auto const& mazeRow : maze) {

		// print top/bottom walls
		for (auto const& cell : mazeRow) {
			std::cout << "+";
			// print if top wall is true
			if (cell.northWall) {
				std::cout << "---";
			}
			else {
				std::cout << "   ";
			}
		}
		std::cout << "+" << std::endl;


		// print left/right walls on next line
		std::cout << "|";
		for (auto const& cell : mazeRow) {
			// print if right wall if true ( x = path, o = searched node but not part of path)
			if (cell.eastWall && cell.path && !cell.search) { // 110
				std::cout << " x |";
			}
			else if (cell.eastWall && !cell.path && !cell.search) { // 100
				std::cout << "   |";
			}
			else if (!cell.eastWall && cell.path && !cell.search) { // 010
				std::cout << " x  ";
			}
			else if (cell.eastWall && cell.path && cell.search) { // 111
				std::cout << " x |";
			}
			else if (cell.eastWall && !cell.path && cell.search) { // 101
				std::cout << " o |";
			}
			else if (!cell.eastWall && !cell.path && cell.search) { // 001
				std::cout << " o  ";
			}
			else if (!cell.eastWall && cell.path && cell.search) { // 011
				std::cout << " x  ";
			}
			else {
				std::cout << "    "; // 000
			}
		}
		std::cout << std::endl;

	}

	// print final bottom walls
	for (int i = 0; i < width; i++) {
		std::cout << "+---";
	}
	std::cout << "+" << std::endl;

}

// depth-first search with backtracker algorithm
void Maze::generateMaze() {
	// set random seed using time
	srand(time(0));

	// create a vector of pointers to Cells
	std::vector<Cell*> mazeStack;

	// create a vector of pointers to neighbours of cells
	std::vector<Cell*> neighbours;

	// choose the initial cell, mark it as visited and push it to stack
	int row = 0;
	int col = 0;
	Cell* currentCell = &maze[row][col];
	currentCell->visited = true;
	mazeStack.push_back(&maze[row][col]);

	// initialise neighbouring cell pointers to something
	Cell* northCell = currentCell;
	Cell* eastCell = currentCell;
	Cell* southCell = currentCell;
	Cell* westCell = currentCell;
	Cell* randomNeighbour = currentCell;

	// while stack is not empty
	while (!mazeStack.empty()) {

		// pop a cell from the stack and make it the current cell
		row = mazeStack.back()->row;
		col = mazeStack.back()->col;
		currentCell = &maze[row][col];
		mazeStack.pop_back();

		// define and check if any neighbours have not been visited
		// also check for out of bound indexes
		if (row > 0 && !maze[row - 1][col].visited) {
			northCell = &maze[row - 1][col];
			neighbours.push_back(northCell);
		}
		if (col < width - 1 && !maze[row][col + 1].visited) {
			eastCell = &maze[row][col + 1];
			neighbours.push_back(eastCell);
		}
		if (row < height - 1 && !maze[row + 1][col].visited) {
			southCell = &maze[row + 1][col];
			neighbours.push_back(southCell);
		}
		if (col > 0 && !maze[row][col - 1].visited) {
			westCell = &maze[row][col - 1];
			neighbours.push_back(westCell);
		}

		// check if any neighbours have not been visited
		if (!neighbours.empty()) {
			// push the current cell to the stack
			mazeStack.push_back(currentCell);
			// choose random unvisited neighbour
			randomNeighbour = neighbours[rand() % neighbours.size()];
			// push chosen cell as visited and push to stack
			randomNeighbour->visited = true;
			mazeStack.push_back(randomNeighbour);
			// remove wall between current cell and chosen cell
			if (randomNeighbour == northCell) {
				currentCell->northWall = false;
			}
			else if (randomNeighbour == eastCell) {
				currentCell->eastWall = false;
			}
			else if (randomNeighbour == southCell) {
				southCell->northWall = false;
			}
			else {
				westCell->eastWall = false;
			}
			neighbours.clear();

		}
	}
	std::cout << "Maze has been generated!\n";
}

// A* algorithm
void Maze::solveMaze() {

	// set of nodes to be evaluated
	std::vector<Cell*> openList;

	// set of nodes already evaluated
	std::vector<Cell*> closedList;

	// set target node
	int targetRow = height - 1;
	int targetCol = width - 1;
	Cell* targetNode = &maze[targetRow][targetCol];

	// set start and current node
	int startRow = 0;
	int startCol = 0;
	Cell* startNode = &maze[startRow][startCol];
	Cell* currentNode = startNode;

	// print out start and end coordinates
	std::cout << "\nStart coordinates are: " << startRow << ", " << startCol << std::endl;
	std::cout << "Target coordinates are: " << targetRow << ", " << targetCol << std::endl;

	// add current node to open list
	openList.push_back(currentNode);
	currentNode->open = true;

	// initialise neighbouring cell pointers to nullptr
	Cell* northNode{ nullptr };
	Cell* eastNode{ nullptr };
	Cell* southNode{ nullptr };
	Cell* westNode{ nullptr };
	Cell* neighbourNode{ nullptr };

	// set some variables
	int currentLowestfCost = 0;
	int currentLowestfCostIndex = 0;

	// loop
	while (1) {
		
		// set current node to node with lowest fCost in open list
		currentLowestfCost = openList[0]->fCost;
		currentLowestfCostIndex = 0;
		for (unsigned int i = 1; i < openList.size(); i++ ) {
			if (openList[i]->fCost < currentLowestfCost) {
				currentLowestfCost = openList[i]->fCost;
				currentLowestfCostIndex = i;
			}
		}
		currentNode = openList[currentLowestfCostIndex];

		// remove current node from open list
		openList.erase(openList.begin() + currentLowestfCostIndex);
		currentNode->open = false;

		// add current node to closed list
		closedList.push_back(currentNode);
		currentNode->closed = true;

		// escape loop if current node is equal to target node
		if (currentNode == targetNode) {
			break;
		}
		
		// for each neighbour (4 adjacent) of the current [row][col]
		for (int j = -1; j < 2; j++) {
			for (int i = -1; i < 2; i++) {
				// skip if diagonal neighbour or current node
				if (i == j || i == -j || (i == 0 && j == 0)) {
					continue;
				}
				// skip if neighbour is out of bounds
				if (currentNode->row + i < 0 || currentNode->col + j < 0 || currentNode->row + i >= height || currentNode->col + j >= width) {
					continue;
				}
				// skip if neighbour is in closed list
				if (maze[currentNode->row + i][currentNode->col + j].closed) {
					continue;
				}
				// skip if current neighbour is north and north wall exists
				if (i == -1 && j == 0 && maze[currentNode->row][currentNode->col].northWall ) {
					continue;
				}
				// skip if current neighbour is east and east wall exists
				if (i == 0 && j == 1 && maze[currentNode->row][currentNode->col].eastWall) {
					continue;
				}
				// skip if current neighbour is south and south wall exists
				if (i == 1 && j == 0 && maze[currentNode->row + 1][currentNode->col].northWall) {
					continue;
				}
				// skip if current neighbour is west and west wall exists
				if (i == 0 && j == -1 && maze[currentNode->row][currentNode->col - 1].eastWall) {
					continue;
				}

				// set gCost of neighbour
				neighbourNode = &maze[currentNode->row + i][currentNode->col + j];
				neighbourNode->setGcost(currentNode->gCost);

				// if new path to neighbour is shorter or neighbour is not in open list
				if (neighbourNode->gCost < currentNode->gCost || !neighbourNode->open) {
					// set fCost of neighbour
					neighbourNode->setHcost(startRow, startCol, targetRow, targetCol);
					neighbourNode->setFcost();
					// set parent of neighbour to current
					neighbourNode->parent = currentNode;
					neighbourNode->search = true;
					// if neighbour is not in open list
					if (!neighbourNode->open) {
						neighbourNode->open = true;
						openList.push_back(neighbourNode);
					}
				}
			}
		}
	}

	// maze has been solved
	// now backtrack from target node to find path and store solution
	int deltaRow = 0;
	int deltaCol = 0;
	currentNode = targetNode;
	while (currentNode != startNode) {

		deltaRow = currentNode->parent->row - currentNode->row;
		deltaCol = currentNode->parent->col - currentNode->col;

		// if backtracking to north node
		if (deltaRow == -1 && deltaCol == 0) {
			solution.push_back("South");
		}
		// else if backtracking to east node
		else if (deltaRow == 0 && deltaCol == 1) {
			solution.push_back("West");
		}
		// else if backtracking to south node
		else if (deltaRow == 1 && deltaCol == 0) {
			solution.push_back("North");
		}
		// else must be backtracking to west node
		else {
			solution.push_back("East");
		}

		currentNode->path = true;
		currentNode = currentNode->parent;
	}
	startNode->path = true;
	std::reverse(solution.begin(), solution.end());
	std::cout << "Maze has been solved!\n";
	std::cout << "x = path, o = search area\n";

}

// save solution to maze in a txt file
void Maze::saveSolutionToTxtFile() {

	std::ofstream solutionFile("Maze_Solution.txt");
	if (solutionFile.is_open()) {

		solutionFile << "The solution to the maze is: \n";
		for (int i = 0; i < solution.size(); i++) {
			solutionFile << solution[i] << std::endl;
		}
		solutionFile.close();
	}
	else std::cout << "Unable to open file.\n";

}