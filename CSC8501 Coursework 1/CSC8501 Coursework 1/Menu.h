#pragma once
#include <string>

class Menu {
public:
	Menu() {};
	~Menu() {};

	static int runMenu();

private:
	static int getMenuChoice();
	static void generateNewMaze();
	static void loadExistingMaze();
	static int getMazeSize();
	static int getMazeExitCount(int size);
	static bool saveRequested();
	static std::string getNewName();
	static std::string getExistingName();
	static bool integerInputted();
};
