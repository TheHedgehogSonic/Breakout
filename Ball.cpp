#include "Ball.h"

ALLEGRO_COLOR Ball::color = al_map_rgb(0xFF, 0xFF, 0xFF);
float Ball::speedCap = 0;
bool Ball::speedState = false;
bool Ball::fire = false;

Ball::Ball(const Ball& clone) {
	this->xPos = clone.xPos;
	this->yPos = clone.yPos;
	this->xSpeed = rand() % 11 - 5; // Different horizontal speeds for malleability
	this->ySpeed = clone.ySpeed - (rand() % 4); // Same with vertical speeds
	this->width = clone.width;
	this->height = clone.height;
}

Ball::~Ball() {
	// Destructor
}

void Ball::draw() {
	al_draw_filled_rectangle(xPos, yPos, xPos + width, yPos + height, color);
}

float Ball::getSpeedCap() {
	return speedCap;
}

void Ball::setSpeedCap(float set) {
	speedCap = set;
}

bool Ball::inSpeedState() {
	return speedState;
}

void Ball::addToSpeedCap(float add) {
	speedCap += add;
	speedCap = speedCap > -10 ? -10 : speedCap; // NEW: Ensures the balls never go too slow
}

void Ball::setSpeedState(bool set) {
	speedState = set;
}

bool Ball::isFire() {
	return fire; // HAHA, because it sounds like someone in a space shooter getting revenge
}

void Ball::setFire(bool set) {
	fire = set;

	if (fire) {
		color = al_map_rgb(0xFF, 0x4F, 0x24);
	}
	else {
		color = al_map_rgb(0xFF, 0xFF, 0xFF);
	}
}

void Ball::checkForSpeedCap() {
	ySpeed = (ySpeed < speedCap ? speedCap : ySpeed);
	ySpeed = (ySpeed > 2 ? 2 : ySpeed);
}

void Ball::print() {
	cout << "Ball Stats:" << endl;
	cout << "X Position is: " << xPos << endl;
	cout << "Y Position is: " << yPos << endl;
	cout << "Horizontal Speed is: " << xSpeed << endl;
	cout << "Vertical Speed is: " << ySpeed << endl;
	cout << "Width is: " << width << endl;
	cout << "Height is: " << height << endl;
}
