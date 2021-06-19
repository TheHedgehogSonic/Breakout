#include "Paddle.h"

Paddle::~Paddle() {
	numObjects--;
}

void Paddle::draw() {
	al_draw_filled_rectangle(xPos, yPos, xPos + width, yPos + height, al_map_rgb(0xFF, 0xFF, 0xFF));
}

void Paddle::print() {
	cout << "Paddle Stats:" << endl;
	cout << "X Position is: " << xPos << endl;
	cout << "Y Position is: " << yPos << endl;
	cout << "Horizontal Speed is: " << xSpeed << endl;
	cout << "Vertical Speed is: " << ySpeed << endl;
	cout << "Width is: " << width << endl;
	cout << "Height is: " << height << endl;
}
