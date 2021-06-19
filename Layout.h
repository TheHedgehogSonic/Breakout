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
	short getLives();
	short getCombo();
	bool isActive();
	void addToScore(unsigned short add); // Adjust stats
	void addToLevel(unsigned short add);
	void addToLives(short add);
	void addToCombo();
	void resetCombo();
	void resetStats(); // Reset stats
	void setupBricks(short rowStart, short colStart); // Susceptible to change in the future
	void eraseBrick(short id);
	short getBrickSize();
	bool checkForWinner();
	void print(); // For debug purposes
private:
	vector<Brick> bricks;
	int score;
	short level;
	short lives;
	short combo;
	bool active; // Check to see if the game is over or not
};