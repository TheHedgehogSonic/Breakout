#pragma once
#include "GameObject.h"

class Brick : public GameObject {
public:
	Brick() : GameObject() { color = 1; /* Set to red by default */ points = 0;  } // Default constructor
	Brick(float x, float y, float w, float h, short c, short p) : GameObject(x, y, w, h) { color = c; points = p; }
	void draw();
	static void initializeSprites(); // INITIALIZATION OF SPRITES COMES AFTER INITIALIZATION OF SCREEN
	static short getWidth(); // Getters and setters
	static short getHeight();
	short getPoints(); // Final, so it doesn't have setter
	void print();
private:
	static ALLEGRO_BITMAP* sprite[6];
	short color; // The color of the brick to use when loaded in
	short points; // The amount of points rewarded upon breaking the brick
};