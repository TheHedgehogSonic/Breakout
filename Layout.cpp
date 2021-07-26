#include "Layout.h"

Layout::Layout() {
	this->score = 0;
	this->level = 1;
	this->shifts = 0;
	this->spawnFrequency = 5;
	this->colorToSpawn = 4; // Green
	this->lives = 5;
	this->combo = 0;
	this->active = true;
}

Layout::Layout(int s, short l, short a) {
	this->score = s;
	this->level = l;
	this->shifts = 0;
	this->spawnFrequency = 5;
	this->colorToSpawn = 4;
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

short Layout::getShifts() {
	return shifts;
}

short Layout::getSpawnFrequency() {
	return spawnFrequency;
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

void Layout::addToScore(int add) {
	score += add;
	score = score > 2000000000 ? 2000000000 : score; // NEW: Checks to see if the score overflowed
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
	shifts = 0;
	colorToSpawn = 4;
	spawnFrequency = 5;
	lives = 5;
	active = true;
}

void Layout::setupBricks(float rowStart, short colStart, short algorithm, short presetColor,  bool presetFast) {
	switch (algorithm) {
	case 1:
		for (short rows = rowStart, c = 1, speed = 1; rows < rowStart + Brick::getHeight() * 6; rows += Brick::getHeight(), c++) { // Rows
			for (short cols = colStart; cols < colStart + Brick::getWidth() * 20; cols += Brick::getWidth()) { // Columns
				bricks.push_back(Brick(cols, rows, Brick::getWidth(), Brick::getHeight(), c, 70 - c * 10, speed));
			}

			if (rows == rowStart + Brick::getHeight() * 2) {
				speed--;
			}
		}
		break;
	case 2: // Just spawn single rows
		for (short cols = colStart; cols < colStart + Brick::getWidth() * 20; cols += Brick::getWidth()) {
			bricks.push_back(Brick(cols, rowStart, Brick::getWidth(), Brick::getHeight(), presetColor, 70 - presetColor * 10, presetFast));
		}
	}
}

void Layout::shiftBricks() {
	shifts++;

	switch (shifts) { // Upping the frequency of brick spawns
	case 4:
		spawnFrequency = 4;
		break;
	case 8:
		spawnFrequency = 3;
		break;
	case 14:
		spawnFrequency = 2;
		break;
	case 22:
		spawnFrequency = 1;
	}
	// Shift the bricks
	for (Brick& b : bricks) {
		b.addToYPos(Brick::getHeight());
	}
	// Spawn process
	if (shifts % 4 < 1 || shifts % 4 > 2) { // 3rd and 4nd shift
		setupBricks(170, 350, 2, colorToSpawn, shifts > 16);
	}
	// Setting up the next color
	if (shifts % 4 == 0) {
		colorToSpawn--;
		colorToSpawn = (colorToSpawn <= 0 ? 6 : colorToSpawn);
	}
	// Level up every time the colors cycle through
	if (shifts % 24 == 16) {
		level++;
	}
	// Deletion process
	bricks.erase(
		remove_if(bricks.begin(), bricks.end(),
			[=](Brick& b) {
				return b.getYPos() > 170 + Brick::getHeight() * 9; // Detection for when a brick goes too low
			}
		),
		bricks.end()
				);
}

void Layout::eraseBrick(short id) {
	bricks.erase(bricks.begin() + id);
}

void Layout::clearBricks() {
	bricks.clear();
}

size_t Layout::getBrickSize() {
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
