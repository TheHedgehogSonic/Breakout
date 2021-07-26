#pragma once
#include "GameObject.h"

class Paddle : public GameObject {
public:
	Paddle() : GameObject() {} // Default constructor
	Paddle(float x, float y, float w, float h) : GameObject(x, y, w, h) {} // Conditional constructor
	~Paddle(); // Destructor
	void draw(); // Shows to the screen
	static short getHits();
	static bool inShortState(); // Checks to see if the paddle has shrunk from the ceiling
	static void addToHits(short add);
	static void setHits(short set);
	static void setShortState(bool set); // Setter for if the paddle can shrink
	void addToWidth(float add);
	void print(); // Debug data
private:
	static short hits; // NEW: Counts how many times paddles hit a ball
	static bool shortState; // NEW: Detects if the paddle shrunk by hitting the ceiling
};