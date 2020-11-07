#pragma once
#include <vector>
#include <string>

struct MazeTile {
	char contents = ' ';
	bool visited = false;
	int bestLength;
	MazeTile* bestParent;
	int x;
	int y;
};

class Maze {
public:
	Maze(int sizeX, int sizeY, int exitCount);
	~Maze();

	std::vector<MazeTile*> getNeighbours(MazeTile* tile);

	int sizeX;
	int sizeY;
	int exitCount;
	std::vector<std::vector<MazeTile*>> tiles;
};