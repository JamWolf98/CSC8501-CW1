#pragma once
#include <vector>

struct MazeTile;
class Maze;

class MazeGenerator {
public:
	MazeGenerator() {};
	~MazeGenerator() {};

	static Maze* generateMaze(int sizeX, int sizeY, int exitCount);
	static MazeTile* generateExit(Maze* maze, std::vector<MazeTile*> borderTiles, bool* successful);
	static void generateRoute(Maze* maze, MazeTile* exit);
	static bool saveMaze(Maze* maze, std::string name);
	static Maze* loadMaze(std::string name);
	static void displayMaze(Maze* maze);
};
