#pragma once
// description of how the maze will be displayed

struct Cell {
	bool visited = false;
	int row = 0;
	int col = 0;

	// only need to manipulate 2 walls for each cell since cells share walls
	bool northWall = true;
	bool eastWall = true;

	// for A* algorithm
	bool open = false;
	bool closed = false;
	int hCost = 0; // heuristic (estimated) cost of moving from current cell to final cell
	int gCost = 0;  // cost of moving from initial cell to current cell
	int fCost = 0; // gCost + hCost
	Cell* parent = nullptr;
	bool path = false;
	bool search = false;

	void setGcost(const int currentgCost) {
		gCost = currentgCost + 1;
	}

	void setHcost(const int startRow, const int startCol, const int targetRow, const int targetCol) {
		hCost = (targetRow - startRow) + (targetCol - startCol);
	}

	void setFcost() {
		fCost = gCost + hCost;
	}

};


//+---+---+---+---+---+
//|   |	  |   |   |   |	
//+---+---+---+---+---+
//|   |	  |   |   |   |	
//+---+---+---+---+---+
//|   |	  |   |   |   |	
//+---+---+---+---+---+
//|   |	  |   |   |   |	
//+---+---+---+---+---+
//|   |	  |   |   |   |	
//+---+---+---+---+---+

//
//     -1,0
// 0,-1    0,+1
//     +1,0
//
//       4, 5
//5, 4   5, 5    5, 6
//       6, 5
//
//
//(currentNode, NeighbourNode)
