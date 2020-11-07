#include <iostream>
#include <fstream>
#include "Menu.h"
#include "Maze.h"
#include "MazeGenerator.h"

int Menu::getMenuChoice() {
	int choice = 0;

	std::cout << "\n1. Generate new maze\n2. Load existing maze\n3. Exit\n\n";

	std::cout << "Enter option: ";
	std::cin >> choice;

	if (integerInputted()) {
		switch (choice) {
		case 1:
		case 2:
		case 3:
			return choice;
		}
	}

	return getMenuChoice();
}

void Menu::generateNewMaze() {
	std::cout << "MAZE WIDTH\n";
	int sizeX = getMazeSize();
	std::cout << "\nMAZE HEIGHT\n";
	int sizeY = getMazeSize();

	std::cout << "\nNUMBER OF EXITS";
	int maxExitCount = ((round((double)sizeX / 2) - 1) * 2) + ((round((double)sizeY / 2) - 1) * 2);
	std::cout << "\nMaximum number of exits for this maze size is " << maxExitCount << ".\n";
	int exitCount = getMazeExitCount(maxExitCount);

	Maze* maze = MazeGenerator::generateMaze(sizeX, sizeY, exitCount);
	MazeGenerator::displayMaze(maze);

	if (saveRequested()) {
		bool successful = false;

		do {
			successful = MazeGenerator::saveMaze(maze, getNewName());
		} while (!successful);
	}

	delete maze;
}

void Menu::loadExistingMaze() {
	std::string name = getExistingName();

	if (!name.empty()) {
		Maze* maze = MazeGenerator::loadMaze(name);
		MazeGenerator::displayMaze(maze);
		delete maze;
	}
}

int Menu::getMazeSize() {
	int size = 0;
	bool validInput = true;

	std::cout << "Enter maze side length: ";
	std::cin >> size;

	validInput = integerInputted();

	if (size < 5) {
		std::cout << "Maze size must be at least 5.\n";
		validInput = false;
	}

	if (validInput) {
		return size;
	}

	return getMazeSize();
}

int Menu::getMazeExitCount(int maxExitCount) {
	int exitCount = 0;
	bool validInput = true;

	std::cout << "Enter number of exits: ";
	std::cin >> exitCount;

	validInput = integerInputted();

	if (exitCount < 1) {
		std::cout << "Maze must have at least 1 exit.\n";
		validInput = false;
	}

	if (exitCount > maxExitCount) {
		std::cout << "Maximum number of exits for this maze size is " << maxExitCount << ".\n";
		validInput = false;
	}

	if (validInput) {
		return exitCount;
	}

	return getMazeExitCount(maxExitCount);
}

bool Menu::saveRequested() {
	std::string save = "";

	std::cout << "Would you like to save this maze?: ";
	std::cin >> save;

	for (auto& letter : save) {
		letter = std::tolower(letter);
	}

	if (save == "yes") {
		return true;
	}

	if (save == "no") {
		return false;
	}

	std::cout << "Invalid input.\n";

	return saveRequested();
}

std::string Menu::getNewName() {
	std::string name = "";

	std::cout << "\nEnter name of new maze: ";
	std::cin >> name;

	return name;
}

std::string Menu::getExistingName() {
	std::string name = "";

	std::cout << "Enter name of existing maze: ";
	std::cin >> name;

	std::ifstream file(name + ".txt");

	if (file.fail()) {
		std::cout << "No maze found with that name.\n";
		name.clear();
	}

	file.close();

	return name;
}

bool Menu::integerInputted() {
	if (std::cin.fail()) {
		std::cout << "Input must be an integer.\n";
		std::cin.clear();
		std::cin.ignore(1024, '\n');
		return false;
	}

	return true;
}

int Menu::runMenu() {	
	int choice = getMenuChoice();	

	std::cout << '\n';

	switch(choice) {
	case 1:
		generateNewMaze();
		break;
	case 2:
		loadExistingMaze();
		break;
	case 3:
		return 0;;
	}

	return runMenu();
}