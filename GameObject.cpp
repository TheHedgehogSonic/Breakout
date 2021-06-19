#include "GameObject.h"

short GameObject::numObjects = 0;
bool GameObject::paused = false;

GameObject::GameObject() {
	numObjects++;
	this->xPos = 0;
	this->yPos = 0;
	this->width = 0;
	this->height = 0;
	this->xSpeed = 0;
	this->ySpeed = 0;
}

GameObject::GameObject(float x, float y, float w, float h) {
	numObjects++;
	this->xPos = x;
	this->yPos = y;
	this->width = w;
	this->height = h;
	this->xSpeed = 0;
	this->ySpeed = 0;
}

GameObject::~GameObject() {
	// Destructor
}

void GameObject::move() {
	if (!paused) {
		xPos += xSpeed;
		yPos += ySpeed;
	}
}

bool GameObject::isPaused() {
	return paused;
}

float GameObject::getXPos() {
	return xPos;
}

float GameObject::getXMiddle() {
	return xPos + width / 2;
}

float GameObject::getRightSide() {
	return xPos + width;
}

float GameObject::getYPos() {
	return yPos;
}

float GameObject::getYMiddle() {
	return yPos + height / 2;
}

float GameObject::getBottom() {
	return yPos + height;
}

float GameObject::getWidth() {
	return width;
}

float GameObject::getHeight() {
	return height;
}

float GameObject::getXSpeed() {
	return xSpeed;
}

float GameObject::getYSpeed() {
	return ySpeed;
}

void GameObject::setPaused(bool set) {
	paused = set;
}

void GameObject::togglePaused() {
	paused = !paused;
}

void GameObject::setXPos(float set) {
	xPos = set;
}

void GameObject::setXMiddle(float set) {
	xPos = set - width / 2;
}

void GameObject::setRightSide(float set) {
	xPos = set - width;
}

void GameObject::setYPos(float set) {
	yPos = set;
}

void GameObject::setYMiddle(float set) {
	yPos = set - height / 2;
}

void GameObject::setBottom(float set) {
	yPos = set - height;
}

void GameObject::addToWidth(float add) {
	width += add;
}

void GameObject::addToHeight(float add) {
	height += add;
}

void GameObject::setWidth(float set) {
	width = set;
}

void GameObject::setHeight(float set) {
	height = set;
}

void GameObject::addToXSpeed(float add) {
	xSpeed += add;
}

void GameObject::addToYSpeed(float add) {
	ySpeed += add;
}

void GameObject::setXSpeed(float set) {
	xSpeed = set;
}

void GameObject::setYSpeed(float set) {
	ySpeed = set;
}

void GameObject::reverseXSpeed() {
	xSpeed = -xSpeed;
}

void GameObject::reverseYSpeed() {
	ySpeed = -ySpeed;
}

void GameObject::print() {
	// This parent function should not be called
}
