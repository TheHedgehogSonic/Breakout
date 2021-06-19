#include "Ball.h"

float Ball::speedCap = 0;
short Ball::ballIDtoAssign = 0;

Ball::~Ball() {
	numObjects--;
}

void Ball::draw() {
	al_draw_filled_rectangle(xPos, yPos, xPos + width, yPos + height, al_map_rgb(0xFF, 0xFF, 0xFF));
}

float Ball::getSpeedCap() {
	return speedCap;
}

short Ball::getID() {
	return ballID;
}

void Ball::setSpeedCap(float set) {
	speedCap = set;
}

void Ball::addToSpeedCap(float add) {
	speedCap += add;
}

void Ball::checkForSpeedCap() {
	ySpeed = (ySpeed < speedCap ? speedCap : ySpeed);
}

void Ball::setID(short set) {
	ballIDtoAssign = set;
}

void Ball::addToID(short add) {
	ballIDtoAssign += add;
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
