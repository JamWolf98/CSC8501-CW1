#include "Maze.h"

Maze::Maze(int sizeX, int sizeY, int exitCount) : sizeX(sizeX), sizeY(sizeY), exitCount(exitCount) {
	tiles.resize(sizeX);

	for (int i = 0; i < sizeX; i++) {
		tiles[i].resize(sizeY);

		for (int j = 0; j < sizeY; j++) {
			tiles[i][j] = new MazeTile();
			tiles[i][j]->bestLength = sizeX * sizeY;
			tiles[i][j]->bestParent = tiles[0][0];
			tiles[i][j]->x = i;
			tiles[i][j]->y = j;
		}
	}
}

Maze::~Maze() {
	for (int i = 0; i < sizeX; i++) {
		for (int j = 0; j < sizeY; j++) {
			delete tiles[i][j];
		}
	}
}

std::vector<MazeTile*> Maze::getNeighbours(MazeTile* tile) {
	std::vector<MazeTile*> neighbours;

	if (tile->x > 0) {
		neighbours.emplace_back(tiles[tile->x - 1][tile->y]);
	}

	if (tile->y > 0) {
		neighbours.emplace_back(tiles[tile->x][tile->y - 1]);
	}

	if (tile->x < sizeX - 1) {
		neighbours.emplace_back(tiles[tile->x + 1][tile->y]);
	}

	if (tile->y < sizeY - 1) {
		neighbours.emplace_back(tiles[tile->x][tile->y + 1]);
	}

	return neighbours;
}
