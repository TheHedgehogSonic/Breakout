#include "Paddle.h"

short Paddle::hits = 0;
bool Paddle::shortState = false;

Paddle::~Paddle() {
	// Destructor
}

void Paddle::draw() {
	al_draw_filled_rectangle(xPos, yPos, xPos + width, yPos + height, al_map_rgb(0xFF, 0xFF, 0xFF));
}

short Paddle::getHits() {
	return hits;
}

bool Paddle::inShortState() {
	return shortState;
}

void Paddle::addToHits(short add) {
	hits += add;
}

void Paddle::setHits(short set) {
	hits = set;
}

void Paddle::setShortState(bool set) {
	shortState = set;
}

void Paddle::addToWidth(float add) {
	width += add;
	width = (width < 30 ? 30 : width);
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
