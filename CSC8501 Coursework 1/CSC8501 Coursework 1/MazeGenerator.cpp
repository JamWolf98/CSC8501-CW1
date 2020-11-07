#include <iostream>
#include <fstream>
#include <vector>
#include "MazeGenerator.h"
#include "Maze.h"

Maze* MazeGenerator::generateMaze(int sizeX, int sizeY, int exitCount) {
	Maze* maze = new Maze(sizeX, sizeY, exitCount);

	int centreX = maze->sizeX / 2;
	int centreY = maze->sizeY / 2;

	for (int i = 1; i < maze->sizeX - 1; i += 2) {
		for (int j = 1; j < maze->sizeY - 1; j += 2) {
			int r = rand() % 2;

			if (r == 0 && i != 1) {
				maze->tiles[i][j - 1]->contents = 'X';
			}
			else if (r == 1 && j != 1) {
				maze->tiles[i - 1][j]->contents = 'X';
			}
		}
	}

	for (int i = centreX - 1; i < centreX + 2; i++) {
		for (int j = centreY - 1; j < centreY + 2; j++) {
			maze->tiles[i][j]->contents = ' ';
		}
	}

	maze->tiles[centreX][centreY]->contents = 'S';

	std::vector<MazeTile*> borderTiles;

	for (int i = 0; i < sizeX; i++) {
		maze->tiles[i][0]->contents = 'X';
		maze->tiles[i][sizeY - 1]->contents = 'X';

		borderTiles.emplace_back(maze->tiles[i][0]);
		borderTiles.emplace_back(maze->tiles[i][sizeY - 1]);
	}

	for (int i = 0; i < sizeY; i++) {
		maze->tiles[0][i]->contents = 'X';
		maze->tiles[sizeX - 1][i]->contents = 'X';

		borderTiles.emplace_back(maze->tiles[0][i]);
		borderTiles.emplace_back(maze->tiles[sizeX - 1][i]);
	}

	MazeTile* exit;
	std::vector<MazeTile*> exits;
	
	for (int i = 0; i < maze->exitCount; i++) {
		bool successful;
		exit = generateExit(maze, borderTiles, &successful);

		if (successful) {
			exits.emplace_back(exit);
		}
		else {
			std::cout << "\nCould not place an exit.";
		}
	}

	for (auto& ex : exits) {
		generateRoute(maze, ex);
	}

	return maze;
}

MazeTile* MazeGenerator::generateExit(Maze* maze, std::vector<MazeTile*> borderTiles, bool* successful) {
	int x = rand() % borderTiles.size();
	auto a = std::find(borderTiles.begin(), borderTiles.end(), borderTiles[x]);
	MazeTile* tile = borderTiles[x];

	borderTiles.erase(a);

	std::vector<MazeTile*> neighbours = maze->getNeighbours(tile);

	bool validExit = false;

	for (auto& n : neighbours) {
		if (n->contents == ' ') {
			validExit = true;
		}
		else if (n->contents == 'E') {
			validExit = false;
			break;
		}
	}

	if (neighbours.size() != 3) {
		validExit = false;
	}

	if (tile->contents == 'E') {
		validExit = false;
	}

	if (validExit) {
		tile->contents = 'E';
		*successful = true;
		return tile;
	}

	if (borderTiles.empty()) {
		*successful = false;
		return tile;
	}

	return generateExit(maze, borderTiles, successful);
}

void MazeGenerator::generateRoute(Maze* maze, MazeTile* exit) {
	bool exitFound = false;
	int centreX = maze->sizeX / 2;
	int centreY = maze->sizeY / 2;

	std::vector<MazeTile*> nextTiles;
	maze->tiles[centreX][centreY]->bestLength = 0;
	nextTiles.emplace_back(maze->tiles[centreX][centreY]);
	MazeTile* currentTile;

	do {
		currentTile = nextTiles.front();
		currentTile->visited = true;

		if (currentTile == exit) {
			exitFound = true;
		}
		else {
			std::vector<MazeTile*> neighbours = maze->getNeighbours(currentTile);

			for (auto& n : neighbours) {
				if (!n->visited && n->contents != 'X' && !(n->contents == 'E' && n != exit) && currentTile->bestLength < n->bestParent->bestLength) {
					n->bestParent = currentTile;
					n->bestLength = n->bestParent->bestLength + 1;
					nextTiles.emplace_back(n);
				}
			}

			auto a = std::find(nextTiles.begin(), nextTiles.end(), currentTile);

			if (a != nextTiles.end()) {
				nextTiles.erase(a);
			}
		}
	} while (!exitFound);

	MazeTile* routeTile = currentTile;

	while (routeTile != maze->tiles[centreX][centreY]) {
		routeTile = routeTile->bestParent;

		if (routeTile->contents != 'S') {
			routeTile->contents = 'o';
		}
	}

	for (int i = 0; i < maze->sizeX; i++) {
		for (int j = 0; j < maze->sizeY; j++) {
			maze->tiles[i][j]->bestParent = maze->tiles[0][0];
			maze->tiles[i][j]->bestLength = maze->sizeX * maze->sizeY;
			maze->tiles[i][j]->visited = false;
		}
	}
}

bool MazeGenerator::saveMaze(Maze* maze, std::string name) {
	std::ofstream file(name + ".txt");

	if (file.fail()) {
		std::cout << "Invalid name with your file system.";

		return false;
	}
	else {
		for (int j = 0; j < maze->sizeY; j++) {
			for (int i = 0; i < maze->sizeX; i++) {
				file << maze->tiles[i][j]->contents;
			}

			file << '\n';
		}

		file.close();

		return true;
	}
}

Maze* MazeGenerator::loadMaze(std::string name) {
	std::ifstream file(name + ".txt");
	std::string line;

	std::getline(file, line);
	int sizeX = line.length();
	int sizeY = 0;

	file.clear();
	file.seekg(0);

	while (!file.eof()) {
		std::getline(file, line);

		if (line != "") {
			sizeY++;
		}
	}

	file.clear();
	file.seekg(0);

	Maze* maze = new Maze(sizeX, sizeY, 0);
	int x;
	int y = 0;
	int exitCount = 0;

	while (!file.eof()) {
		std::getline(file, line);
		x = 0;

		for (auto& tile : line) {
			if (tile == 'E') {
				exitCount++;
			}

			maze->tiles[x][y]->contents = tile;
			x++;
		}

		y++;
	}

	file.close();

	maze->exitCount = exitCount;

	return maze;
}

void MazeGenerator::displayMaze(Maze* maze) {
	std::cout << "\n\n";
	
	for (int j = 0; j < maze->sizeY; j++) {
		for (int i = 0; i < maze->sizeX; i++) {
			std::cout << maze->tiles[i][j]->contents;
		}

		std::cout << '\n';
	}

	std::cout << '\n';
}
