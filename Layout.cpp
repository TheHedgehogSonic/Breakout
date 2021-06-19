#include "Layout.h"

Layout::Layout() {
	this->score = 0;
	this->level = 1;
	this->lives = 5;
	this->combo = 0;
	this->active = true;
}

Layout::Layout(int s, short l, short a) {
	this->score = s;
	this->level = l;
	this->lives = a;
	this->combo = 0;
	this->active = true;
}

Layout::~Layout() {
	bricks.clear();
}

Brick Layout::getBrick(short id) {
	return bricks.at(id);
}

int Layout::getScore() {
	return score;
}

short Layout::getLevel() {
	return level;
}

short Layout::getLives() {
	return lives;
}

short Layout::getCombo() {
	return combo;
}

bool Layout::isActive() {
	return active;
}

void Layout::addToScore(unsigned short add) {
	score += add;
}

void Layout::addToLevel(unsigned short add) {
	level += add;
}

void Layout::addToLives(short add) {
	lives += add;
	active = (lives > 0);
}

void Layout::addToCombo() {
	combo++;
}

void Layout::resetCombo() {
	combo = 0;
}

void Layout::resetStats() {
	bricks.clear();
	score = 0;
	level = 1;
	lives = 5;
	active = true;
}

void Layout::setupBricks(short rowStart, short colStart) {
	for (short rows = rowStart, c = 1; rows < rowStart + Brick::getHeight() * 6; rows += Brick::getHeight(), c++) { // Rows
		for (short cols = colStart; cols < colStart + Brick::getWidth() * 20; cols += Brick::getWidth()) {
			bricks.push_back(Brick(cols, rows, Brick::getWidth(), Brick::getHeight(), c, 70 - c * 10));
		}
	}
}

void Layout::eraseBrick(short id) {
	bricks.erase(bricks.begin() + id);
}

short Layout::getBrickSize() {
	return bricks.size();
}

bool Layout::checkForWinner() {
	return !bricks.size();
}

void Layout::print() {
	cout << "Layout Stats:" << endl;
	cout << "Score is: " << score << endl;
	cout << "Level is: " << level << endl;
	cout << "Lives left is: " << lives << endl;
	cout << "Active is set to: " << active << endl;
}
