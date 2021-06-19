#pragma once
#include <fstream>
#include <string>

using namespace std;

class FileIO {
public:
	FileIO();
	~FileIO();
	static int getHighScore();
	static void loadHighScore(); // Load the high score from memory
	static bool saveHighScore(int score); // Save the high score into memory (only writes if score earned is higher, and returns just that)
private:
	static ifstream reader; // File loader
	static ofstream writer; // File saver
	static string filePath; // Path of the file on your computer
	static int highScore; // High score
};