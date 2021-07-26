#pragma once
#include <iostream>
#include <vector>
#include "Brick.h"

using namespace std;

class Layout {
public:
	Layout(); // Default constructor
	Layout(int s, short l, short a); // Conditional constructor
	~Layout(); // Destructor
	Brick getBrick(short id); // Getter for specific bricks
	int getScore(); // Getters for stats
	short getLevel();
	short getShifts();
	short getSpawnFrequency();
	short getLives();
	short getCombo();
	bool isActive();
	void addToScore(int add); // Adjust stats
	void addToLevel(unsigned short add);
	void addToLives(short add);
	void addToCombo();
	void resetCombo();
	void resetStats(); // Reset stats
	void setupBricks(float rowStart, short colStart, short algorithm, short presetColor = 1, bool presetFast = false); // NEW: Sets up bricks in more ways than one
	void shiftBricks(); // NEW: Makes bricks shift downwards
	void eraseBrick(short id);
	void clearBricks();
	size_t getBrickSize();
	bool checkForWinner();
	void print(); // For debug purposes
private:
	vector<Brick> bricks;
	int score;
	short level;
	short shifts; // NEW: A special type of level counter for Endurance Mode
	short spawnFrequency; // NEW: Determines how many hits it takes to
	short colorToSpawn; // NEW: Determining the color of bricks to spawn in Endurance mode
	short lives;
	short combo;
	bool active; // Check to see if the game is over or not
};