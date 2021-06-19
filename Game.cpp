#include <windows.h>
#include <vector>
#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_acodec.h>
#include <allegro5\allegro_audio.h>
#include "Paddle.h"
#include "Ball.h"
#include "Layout.h"
#include "FileIO.h"
//////////////////////////////////////////////////
// This is Breakout. The classic brick breaking //
// game by Atari re-imagined on the big screen  //
// through the power of Allegro.                //
//////////////////////////////////////////////////
using namespace std;
// For color text
const HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
// Typedefs
typedef unsigned short POS;
// Objects
vector<Paddle> paddles(0);
vector<Ball> balls(0);
Layout layout[2]; // Enough to hold 2 players
// Variables
const POS WIDTH = 1600, HEIGHT = 900; // Screen width and height
const char DOBERVICH_BUMPER_STICKER[] = "USE YOUR VARIABLES!"; // Very important
short players = 1; // Tracker for how many people are playing
short activePlayer = 0; // Tracker for the player currently playing
bool running = true; // Checks to see if the game is running
bool pauseLock = false;
// Functions
void switchTurn(); // Switches which player is in control
bool gameOver(); // Speaks for itself
POS rngesus(POS low, POS high); // Easily controllable random number generator
// Game stuff
int main() {
	// assert(DOBERVICH_BUMPER_STICKER == "USE YOUR VARIABLES!"); // Crash the game if Mr. Dobervich's dreams are tampered with
	srand((unsigned) time(0)); // Seeds the RNG based on what time you boot the game up

	paddles.push_back(Paddle(WIDTH / 2, HEIGHT - 50, WIDTH / 16, 10));
	// All the necessary pointers to objects that control the game
	ALLEGRO_DISPLAY* screen; // The screen (Hardware rendered)
	ALLEGRO_EVENT_QUEUE* queue; // Looks for events
	ALLEGRO_TIMER* framerate; // Framerate
	ALLEGRO_FONT* font; // Font used in game
	// Initializing the necessary game runners
	al_init();
	al_init_image_addon();
	al_init_primitives_addon(); // USE THIS TO STOP EXCEPTIONS FROM BLOCKING THE GAME!
	al_install_audio();
	al_init_acodec_addon();

	screen = al_create_display(WIDTH, HEIGHT); // C++ equivalent of size(x, y)
	queue = al_create_event_queue(); // Made to queue events
	framerate = al_create_timer(1.0 / 60); // 1 divided by what you want the framerate to be (60FPS here)
	font = al_create_builtin_font(); // Custom font built in to Allegro
	// Sprites
	Brick::initializeSprites(); // LOAD IN ALL SPRITES AFTER THE DISPLAY TO AVOID NULL POINTER EXCEPTIONS
	// Using the necessary game runners
	al_install_keyboard(); // Initializes the keyboard
	// The checks for all events in-game
	al_register_event_source(queue, al_get_display_event_source(screen)); // Handles screen
	al_register_event_source(queue, al_get_keyboard_event_source()); // Handles key inputs
	al_register_event_source(queue, al_get_timer_event_source(framerate)); // Handles framerate
	// Game loop
	al_start_timer(framerate); // THIS PART IS VERY IMPORTANT OR ELSE THE GAME WON'T RUN
	ALLEGRO_EVENT input; // Handles game events and makes sure the game runs at the right framerate
	ALLEGRO_KEYBOARD_STATE key; // Handles multiple keys at the same time
	// Game variables
	bool inPlay = false; // Detects if balls are in play
	bool message = false; // That's a secret! |:)
	// Music setup
	ALLEGRO_SAMPLE* sounds[7];
	al_reserve_samples(7);

	sounds[0] = al_load_sample("Paddle Bounce.wav");
	sounds[1] = al_load_sample("Wall Bounce.wav");
	sounds[2] = al_load_sample("Brick Hit.wav");
	sounds[3] = al_load_sample("Ball Lost.wav");
	sounds[4] = al_load_sample("Bricks Reloaded.wav");
	sounds[5] = al_load_sample("Power up.wav");
	sounds[6] = al_load_sample("High Score.wav");
	// Other setup
	FileIO::loadHighScore();
	layout[0].setupBricks(200, (WIDTH - HEIGHT) / 2);
	Ball::setSpeedCap(-10);
	// Game loop
	while (running) {
		al_wait_for_event(queue, &input);
		al_get_keyboard_state(&key); // Detects input
		// public void draw() {}
		if (input.type == ALLEGRO_EVENT_TIMER) { // Runs at 60FPS
			al_clear_to_color(al_map_rgb(0x00, 0x00, 0x00));
			al_draw_filled_rectangle(0, 0, (WIDTH - HEIGHT) / 2, HEIGHT, al_map_rgb(0x7F, 0x7F, 0x7F));
			al_draw_filled_rectangle(WIDTH - (WIDTH - HEIGHT) / 2 - 1, 0, WIDTH, HEIGHT, al_map_rgb(0x7F, 0x7F, 0x7F));
			// Text on the left side
			al_draw_textf(font, al_map_rgb(0x00, 0xFF, 0xFF), 340, 40, ALLEGRO_ALIGN_RIGHT, "%d", FileIO::getHighScore());
			al_draw_textf(font, al_map_rgb(0x00, 0xFF, 0xFF), 240, 40, ALLEGRO_ALIGN_RIGHT, "%s", "High Score:");

			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, activePlayer == 0 ? 0x00 : 0xFF), 340, 100, ALLEGRO_ALIGN_RIGHT, "%d", layout[0].getScore());
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, activePlayer == 0 ? 0x00 : 0xFF), 240, 100, ALLEGRO_ALIGN_RIGHT, "%s", "Player 1 Score:");

			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, activePlayer == 0 ? 0x00 : 0xFF), 340, 120, ALLEGRO_ALIGN_RIGHT, "%d", layout[0].getLevel());
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, activePlayer == 0 ? 0x00 : 0xFF), 240, 120, ALLEGRO_ALIGN_RIGHT, "%s", "Player 1 Level:");

			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, activePlayer == 0 ? 0x00 : 0xFF), 340, 140, ALLEGRO_ALIGN_RIGHT, "%d", layout[0].getLives());
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, activePlayer == 0 ? 0x00 : 0xFF), 240, 140, ALLEGRO_ALIGN_RIGHT, "%s", "Player 1 Lives:");

			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, activePlayer == 1 ? 0x00 : 0xFF), 340, 200, ALLEGRO_ALIGN_RIGHT, "%d", layout[1].getScore());
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, activePlayer == 1 ? 0x00 : 0xFF), 240, 200, ALLEGRO_ALIGN_RIGHT, "%s", "Player 2 Score:");

			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, activePlayer == 1 ? 0x00 : 0xFF), 340, 220, ALLEGRO_ALIGN_RIGHT, "%d", layout[1].getLevel());
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, activePlayer == 1 ? 0x00 : 0xFF), 240, 220, ALLEGRO_ALIGN_RIGHT, "%s", "Player 2 Level:");

			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, activePlayer == 1 ? 0x00 : 0xFF), 340, 240, ALLEGRO_ALIGN_RIGHT, "%d", layout[1].getLives());
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, activePlayer == 1 ? 0x00 : 0xFF), 240, 240, ALLEGRO_ALIGN_RIGHT, "%s", "Player 2 Lives:");
			// Credits
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 340, 400, ALLEGRO_ALIGN_RIGHT, "%s", "Original game makers: Atari");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 340, 420, ALLEGRO_ALIGN_RIGHT, "%s", "Sprite creator used: GraphicsGale");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 340, 440, ALLEGRO_ALIGN_RIGHT, "%s", "Music/SFX maker used: Raster Music Tracker");
			// Text on the right side
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 100, ALLEGRO_ALIGN_LEFT, "%s", "This Breakout clone is made by Bennett");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 120, ALLEGRO_ALIGN_LEFT, "%s", "(Not from Genshin Impact) Johnson.");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 140, ALLEGRO_ALIGN_LEFT, "%s", "Break the bricks to win!");

			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 200, ALLEGRO_ALIGN_LEFT, "%s", "Arrow keys: Move paddle");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 220, ALLEGRO_ALIGN_LEFT, "%s", "Right Ctrl: Move faster");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 240, ALLEGRO_ALIGN_LEFT, "%s", "Space: Spawn ball");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 260, ALLEGRO_ALIGN_LEFT, "%s", "NUMPAD 1: Start 1 Player Game");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 280, ALLEGRO_ALIGN_LEFT, "%s", "NUMPAD 2: Start 2 Player Game");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 300, ALLEGRO_ALIGN_LEFT, "%s", "P: Pause");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 320, ALLEGRO_ALIGN_LEFT, "%s", "Set to 1 Player Game by default on boot");
			// Other text
			if (GameObject::isPaused()) {
				al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), WIDTH / 2, 600, ALLEGRO_ALIGN_CENTER, "%s", "Paused");
			}

			if (gameOver()) {
				al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), WIDTH / 2, 600, ALLEGRO_ALIGN_CENTER, "%s", "Game Over");
				al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), WIDTH / 2, 620, ALLEGRO_ALIGN_CENTER, "%s", "Press NUMPAD 1 or 2 to try again.");

				if (message) {
					al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), WIDTH / 2, 500, ALLEGRO_ALIGN_CENTER, "%s", "A new high score!");
					al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), WIDTH / 2, 520, ALLEGRO_ALIGN_CENTER, "%s", "Remember, Mr. Dobervich says to always");
					al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), WIDTH / 2, 540, ALLEGRO_ALIGN_CENTER, "%s", DOBERVICH_BUMPER_STICKER);
				}
			}

			paddles[0].draw(); // Display the paddle to the screen
			paddles[0].move(); // Make the paddle movable
			paddles[0].setXSpeed(0); // Stop the paddle if no key is being held

			for (POS i = 0; i < layout[activePlayer].getBrickSize(); i++) {
				layout[activePlayer].getBrick(i).draw(); // Draw the bricks
			}
			// Key input detector
			if (al_key_down(&key, ALLEGRO_KEY_LEFT)) { // Go left
				paddles[0].setXSpeed(al_key_down(&key, ALLEGRO_KEY_RCTRL) ? -16 : -8); // May be replaced if more paddles are added
			}

			if (al_key_down(&key, ALLEGRO_KEY_RIGHT)) { // Go right
				paddles[0].setXSpeed(al_key_down(&key, ALLEGRO_KEY_RCTRL) ? 16 : 8);
			}

			if (al_key_down(&key, ALLEGRO_KEY_SPACE) && !inPlay && layout[activePlayer].isActive()) { // Spawn the ball
				balls.push_back(Ball(WIDTH / 2, 500, 10, 10)); // Spawn the ball
				Ball::addToID(1); // Update ball ID (If anyone knows a better way that doesn't crash with multiple balls, tell me)
				balls[0].setXSpeed(rngesus(0, 10) - 5); // Set speeds
				balls[0].setYSpeed(5);
				inPlay = true;
			}

			if (al_key_down(&key, ALLEGRO_KEY_PAD_1)) { // If the player wants to start a 1 player game
				activePlayer = 0;
				Ball::setSpeedCap(-10); // Reset ball speed cap
				layout[0].resetStats(); // Reset stats for Player 1
				layout[1].resetStats(); // Reset stats for Player 2
				layout[0].setupBricks(200, (WIDTH - HEIGHT) / 2); // Reset the bricks
				players = 1;
				balls.clear(); // Clear any left over balls
				inPlay = false; // Make the ball spawnable
				message = false;
				Ball::setID(0);
			}

			if (al_key_down(&key, ALLEGRO_KEY_PAD_2)) { // If the player wants to start a 2 player game
				activePlayer = 0;
				Ball::setSpeedCap(-10); // Reset ball speed cap
				layout[0].resetStats(); // Reset stats for Player 1
				layout[1].resetStats(); // Reset stats for Player 2
				layout[0].setupBricks(200, (WIDTH - HEIGHT) / 2); // Reset the bricks
				layout[1].setupBricks(200, (WIDTH - HEIGHT) / 2);
				players = 2;
				balls.clear(); // Clear any left over balls
				inPlay = false; // Make the ball spawnable
				message = false;
				Ball::setID(0);
			}

			if (al_key_down(&key, ALLEGRO_KEY_P) && !pauseLock) { // Pause the game
				GameObject::togglePaused(); // Now there IS a pause in Breakout!
				pauseLock = true; // Ensure pauses don't cycle while key is held
			}

			if (!al_key_down(&key, ALLEGRO_KEY_P)) { // Detect if released
				pauseLock = false; // This completes the cycle of pausing only happening once on key press
			}
			// Ball traits
			for (Ball& b : balls) { // Loops over all balls (MAKE SURE YOU DEREFERENCE!)
				b.draw();
				b.move();
				// Wall collision
				if (b.getXPos() <= (WIDTH - HEIGHT) / 2 && b.getXSpeed() < 0 || b.getRightSide() >= WIDTH - (WIDTH - HEIGHT) / 2 - 1 && b.getXSpeed() > 0) {
					al_play_sample(sounds[1], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
					b.reverseXSpeed(); // Collision detection for balls against walls
				}
				// Ceiling collision
				if (b.getYPos() <= 0 && b.getYSpeed() < 0) {
					al_play_sample(sounds[1], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
					b.reverseYSpeed(); // Collision detection for balls against the ceiling
				}
				// Paddle collision
				for (Paddle p : paddles) { // Loops over all paddles
					if (b.getRightSide() >= p.getXPos() && b.getXPos() <= p.getRightSide() && b.getBottom() >= p.getYPos() && b.getBottom() <= p.getYMiddle() + b.getYSpeed()) {
						al_play_sample(sounds[0], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
						b.reverseYSpeed(); // Collision detection between ball and paddle
						b.addToXSpeed(p.getXSpeed() / 7.5f + (b.getXMiddle() - p.getXMiddle()) / 5);
						b.addToYSpeed(-0.5); // Change in future
						b.checkForSpeedCap();
						layout[activePlayer].resetCombo();

						if (layout[activePlayer].checkForWinner()) { // Check for a winner every time the ball bounces
							al_play_sample(sounds[4], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
							layout[activePlayer].setupBricks(200, (WIDTH - HEIGHT) / 2);
							layout[activePlayer].addToLevel(1);
							layout[activePlayer].addToLives(1); // Susceptible to change
							Ball::addToSpeedCap(-2);
						}
					}
				}
				// Brick collision
				for (POS i = 0; i < layout[activePlayer].getBrickSize(); i++) { // Loop over bricks
					if (b.getYPos() <= layout[activePlayer].getBrick(i).getBottom() - b.getYSpeed() && b.getBottom() >= layout[activePlayer].getBrick(i).getYPos() - b.getYSpeed()) { // Vertical collision detection
						if (b.getXPos() <= layout[activePlayer].getBrick(i).getRightSide() && b.getRightSide() >= layout[activePlayer].getBrick(i).getXPos()) { // Horizontal
							al_play_sample(sounds[2], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
							layout[activePlayer].addToCombo(); // Add to the combo to award more points
							layout[activePlayer].addToScore(layout[activePlayer].getBrick(i).getPoints() * layout[activePlayer].getCombo() * layout[activePlayer].getLevel()); // Award points
							layout[activePlayer].eraseBrick(i); // Delete the brick that got hit (CALL THIS FUNCTION LAST)
							b.reverseYSpeed(); // Bounce the ball
						}
					}
				}

				if (b.getYPos() > HEIGHT) {
					balls.erase(balls.begin() + b.getID()); // Wipe the ball out of memory to make room

					if (balls.size() == 0) {

						layout[activePlayer].addToLives(-1); // Lose a life
						Ball::addToID(-1); // Reset ball ID
						inPlay = false; // Make ball spawn available

						if (!gameOver()) {
							al_play_sample(sounds[3], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
						}

						if (layout[activePlayer + 1 % 2].isActive() && players == 2) {
							switchTurn();
							Ball::setSpeedCap(-8 - 2 * layout[activePlayer].getLevel()); // Set the correct ball vertical speed cap for the player
						}
						// Writing a high score
						if (gameOver()) {
							if (FileIO::saveHighScore(layout[0].getScore())) {
								al_play_sample(sounds[6], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
								FileIO::loadHighScore();
								message = true;
							}

							if (FileIO::saveHighScore(layout[1].getScore())) {
								al_play_sample(sounds[6], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
								FileIO::loadHighScore();
								message = true;
							}
						}
					}
				}
			}

			for (Paddle& p : paddles) { // Paddle loop
				if (p.getXPos() < (WIDTH - HEIGHT) / 2) { // Prevention from going off the left side
					p.setXPos((WIDTH - HEIGHT) / 2);
				}
				else if (p.getRightSide() > WIDTH - (WIDTH - HEIGHT) / 2 - 1) { // Prevention from going off the right side
					p.setRightSide(WIDTH - (WIDTH - HEIGHT) / 2 + 1);
				}
			}

			al_flip_display(); // Goes on bottom
		}
		// Check if the user quit
		if (input.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			running = false;
		}
	}
	// Running destructors (It's good to be responsible with your data)
	al_destroy_display(screen); // They all speak for themselves
	al_destroy_event_queue(queue);
	al_destroy_timer(framerate);
	al_destroy_font(font);
	al_uninstall_keyboard();

	for (POS i = 0; i < 7; i++) {
		al_destroy_sample(sounds[i]);
	}

	al_uninstall_audio(); // PUT THIS AFTER DESTROYING SAMPLES
	return 0;
}
// Switch player turns
void switchTurn() {
	++activePlayer %= 2;
}

bool gameOver() {
	return !layout[0].isActive() && (!layout[1].isActive() || players == 1);
}
// A random number generator named after the god of luck
POS rngesus(POS low, POS high) {
	return rand() % ((high - low) + 1) + low;
}
