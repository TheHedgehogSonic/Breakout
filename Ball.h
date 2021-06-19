#pragma once
#include "GameObject.h"
#include <allegro5/allegro_primitives.h>

class Ball : public GameObject {
public:
	Ball() : GameObject() { ballID = ballIDtoAssign; } // Default constructor
	Ball(float x, float y, float w, float h) : GameObject(x, y, w, h) { ballID = ballIDtoAssign; } // Conditional constructor
	~Ball(); // Destructor
	void draw();
	static float getSpeedCap();
	short getID();
	static void setSpeedCap(float set);
	static void addToSpeedCap(float add);
	void checkForSpeedCap();
	static void setID(short set);
	static void addToID(short add);
	void print();
private:
	static float speedCap; // Prevents your balls from going too fast
	static short ballIDtoAssign; // Unique ID codes are passed to each ball to help identify each.
	short ballID;
};