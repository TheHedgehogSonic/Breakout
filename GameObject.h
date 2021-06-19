#pragma once
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_image.h>
#include <iostream>

using namespace std;

class GameObject abstract {
public:
	explicit GameObject(); // Default constructor
	explicit GameObject(float x, float y, float w, float h); // Conditional constructor
	virtual ~GameObject(); // Destructor
	virtual void draw() abstract; // Shows the sprite on screen
	void move(); // Puts the speed variables into action, and shifts objects depending on them
	static bool isPaused();
	float getXPos(); // Getters for position, speed and sprite size
	float getXMiddle(); // Gets the middle of a sprite instead of the left
	float getRightSide(); // Gets the right side of a sprite
	float getYPos();
	float getYMiddle(); // Gets the middle of a sprite instead of the top
	float getBottom(); // Gets the bottom of a sprite
	float getWidth();
	float getHeight();
	float getXSpeed();
	float getYSpeed();
	static void setPaused(bool set);
	static void togglePaused();
	void setXPos(float set); // Setters for position, speed and sprite size
	void setXMiddle(float set); // Set X-position based on the middle, instead of left
	void setRightSide(float set); // Same with the right side
	void setYPos(float set);
	void setYMiddle(float set); // The same for Y-axis as mentioned above
	void setBottom(float set);
	void addToWidth(float add);
	void addToHeight(float add);
	void setWidth(float set);
	void setHeight(float set);
	void addToXSpeed(float add);
	void addToYSpeed(float add);
	void setXSpeed(float set);
	void setYSpeed(float set);
	void reverseXSpeed(); // Sends object the other way horizontally
	void reverseYSpeed(); // Sends object the other way vertically
	virtual void print(); // This functions differently across all objects, as it prints their stats
protected:
	float xPos; // Horizontal position
	float yPos; // Vertical position (upside down)
	float width; // Sprite width
	float height; // Sprite height
	float xSpeed; // Horizontal speed
	float ySpeed; // Vertical speed (inverted)
	static short numObjects; // A variable for debugging to keep track of used memory
private:
	static bool paused; // And they say there's no pause on Atari 2600 games...
};