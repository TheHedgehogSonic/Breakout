#include "FileIO.h"

ifstream FileIO::reader;
ofstream FileIO::writer;
string FileIO::filePath = "High Score.txt";
int FileIO::highScore = 0;

FileIO::FileIO() {
	// Constructor
}

FileIO::~FileIO() {
	// Destructor
}

int FileIO::getHighScore() {
	return highScore;
}

void FileIO::loadHighScore() {
	reader.open(filePath);
	reader >> highScore;
	reader.close();
}

bool FileIO::saveHighScore(int score) {
	if (score > highScore) {
		writer.open(filePath);
		writer << score;
		writer.close();
		return true;
	}

	return false;
}
