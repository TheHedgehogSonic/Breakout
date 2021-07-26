#pragma once
#include "GameObject.h"

class Powerup : public GameObject {
public:
	Powerup() : GameObject() { type = 0; effect = false; ySpeed = 5; } // Default constructor
	Powerup(float x, float y, float w, float h, short t, bool e) : GameObject(x, y, w, h) { type = t; effect = e; ySpeed = 5; }
	~Powerup(); // Destructor
	void draw(); // Draw the sprite object on the screen
	static void initializeSprites(); // Sets up the sprites
	static void showSprite(float x, float y, short t, bool e); // Show the sprite without constructing an object
	static short getWidth();
	static short getHeight();
	short getType();
	bool getEffect();
private:
	static ALLEGRO_BITMAP* sprite[5][2];
	short type; // The type of powerup you'll recieve when touched
	bool effect; // If the item you get is a power up or a power down
};