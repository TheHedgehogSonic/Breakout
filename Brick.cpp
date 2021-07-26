#include "Brick.h"

ALLEGRO_BITMAP* Brick::sprite[6];

void Brick::draw() {
	switch (color) {
	case 1: // Red
	case 2: // Orange
	case 3: // Yellow
	case 4: // Green
	case 5: // Blue
	case 6: // Purple
		al_draw_bitmap(sprite[color - 1], xPos, yPos, 0);
		break;
	default: // Prevent array index from going out of bounds
		cerr << "ERROR: Using undefined brick color" << endl;
	}
}

void Brick::initializeSprites() {
	sprite[0] = al_load_bitmap("Brick (Red).bmp"); // Red
	sprite[1] = al_load_bitmap("Brick (Orange).bmp"); // Orange
	sprite[2] = al_load_bitmap("Brick (Yellow).bmp"); // Yellow
	sprite[3] = al_load_bitmap("Brick (Green).bmp"); // Green
	sprite[4] = al_load_bitmap("Brick (Blue).bmp"); // Blue
	sprite[5] = al_load_bitmap("Brick (Purple).bmp"); // Purple
}

short Brick::getWidth() {
	return al_get_bitmap_width(sprite[0]);
}

short Brick::getHeight() {
	return al_get_bitmap_height(sprite[0]);
}

bool Brick::isFast() {
	return fastBall;
}

short Brick::getPoints() {
	return points;
}

void Brick::print() {
	cout << "Brick stats are:" << endl;
	cout << "X Position is: " << xPos << endl;
	cout << "Y Position is: " << yPos << endl;
	cout << "Width is: " << width << endl;
	cout << "Height is: " << height << endl;
	cout << "Point value is: " << points << endl;
}
