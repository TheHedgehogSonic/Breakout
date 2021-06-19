#pragma once
#include "GameObject.h"

class Paddle : public GameObject {
public:
	Paddle() : GameObject() {} // Default constructor
	Paddle(float x, float y, float w, float h) : GameObject(x, y, w, h) {} // Conditional constructor
	~Paddle(); // Destructor
	void draw(); // Shows to the screen
	void print(); // Debug data
private:
	// Coming soon
};