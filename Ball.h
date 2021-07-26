#pragma once
#include "GameObject.h"
#include <allegro5/allegro_primitives.h>

class Ball : public GameObject {
public:
	Ball() : GameObject() {} // Default constructor
	Ball(float x, float y, float w, float h) : GameObject(x, y, w, h) {} // Conditional constructor
	Ball(const Ball& clone); // Copy constructor
	~Ball(); // Destructor
	void draw();
	static float getSpeedCap();
	static void setSpeedCap(float set);
	static bool inSpeedState();
	static void addToSpeedCap(float add);
	static void setSpeedState(bool set);
	static bool isFire(); // Getter for fire
	static void setFire(bool set); // Setter for fire
	void checkForSpeedCap();
	void print();
private:
	static ALLEGRO_COLOR color;
	static float speedCap; // Prevents your balls from going too fast
	static bool speedState; // Checks if balls are in fast mode
	static bool fire;
};