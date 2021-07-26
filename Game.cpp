#include <windows.h>
#include <vector>
#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_acodec.h>
#include <allegro5\allegro_audio.h>
#include "Paddle.h"
#include "Ball.h"
#include "Powerup.h"
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
vector<Powerup> powerups(0);
Layout layout[2]; // Enough to hold 2 players
// Variables
const POS WIDTH = 1600, HEIGHT = 900; // Screen width and height
const char DOBERVICH_BUMPER_STICKER[] = "USE YOUR VARIABLES!"; // Very important
bool dipSwitches[3] = { true, true, true }; // Dipswitches -- 0: Atari Ruleset on/off, 1: Items on/off 2: Classic mode/Endurance mode
short players = 1; // Tracker for how many people are playing
short activePlayer = 0; // Tracker for the player currently playing
POS powerRoll; // NEW: The RNG variable that determines which powerup is dropped from a brick
bool running = true; // Checks to see if the game is running
bool dipSwitchLock = false; // Locks to prevent actions from happening more than once
bool toggleLock = false;
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
	Powerup::initializeSprites();
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
	sounds[5] = al_load_sample("Power up.wav"); // Previously unused sound byte
	sounds[6] = al_load_sample("High Score.wav");
	// Other setup
	FileIO::loadHighScore();
	layout[0].setupBricks(200, (WIDTH - HEIGHT) / 2, 1);
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

			if (players < 3) {
				al_draw_textf(font, al_map_rgb(0xFF, 0xFF, activePlayer == 0 ? 0x00 : 0xFF), 340, 100, ALLEGRO_ALIGN_RIGHT, "%d", layout[0].getScore());
				al_draw_textf(font, al_map_rgb(0xFF, 0xFF, activePlayer == 0 ? 0x00 : 0xFF), 240, 100, ALLEGRO_ALIGN_RIGHT, "%s", "Player 1 Score:");

				al_draw_textf(font, al_map_rgb(0xFF, 0xFF, activePlayer == 0 ? 0x00 : 0xFF), 340, 120, ALLEGRO_ALIGN_RIGHT, "%d", layout[0].getLevel());
				al_draw_textf(font, al_map_rgb(0xFF, 0xFF, activePlayer == 0 ? 0x00 : 0xFF), 240, 120, ALLEGRO_ALIGN_RIGHT, "%s", "Player 1 Level:");

				al_draw_textf(font, al_map_rgb(0xFF, 0xFF, activePlayer == 0 ? 0x00 : 0xFF), 340, 140, ALLEGRO_ALIGN_RIGHT, "%d", layout[0].getLives());
				al_draw_textf(font, al_map_rgb(0xFF, 0xFF, activePlayer == 0 ? 0x00 : 0xFF), 240, 140, ALLEGRO_ALIGN_RIGHT, "%s", "Player 1 Lives:");
				// NEW: Display number of brick shifts in Endurance mode
				if (!dipSwitches[2]) {
					al_draw_textf(font, al_map_rgb(0xFF, 0xFF, activePlayer == 0 ? 0x00 : 0xFF), 340, 160, ALLEGRO_ALIGN_RIGHT, "%d", layout[0].getShifts());
					al_draw_textf(font, al_map_rgb(0xFF, 0xFF, activePlayer == 0 ? 0x00 : 0xFF), 240, 160, ALLEGRO_ALIGN_RIGHT, "%s", "Player 1 Brick Shifts:");
				}

				al_draw_textf(font, al_map_rgb(0xFF, 0xFF, activePlayer == 1 ? 0x00 : 0xFF), 340, 200, ALLEGRO_ALIGN_RIGHT, "%d", layout[1].getScore());
				al_draw_textf(font, al_map_rgb(0xFF, 0xFF, activePlayer == 1 ? 0x00 : 0xFF), 240, 200, ALLEGRO_ALIGN_RIGHT, "%s", "Player 2 Score:");

				al_draw_textf(font, al_map_rgb(0xFF, 0xFF, activePlayer == 1 ? 0x00 : 0xFF), 340, 220, ALLEGRO_ALIGN_RIGHT, "%d", layout[1].getLevel());
				al_draw_textf(font, al_map_rgb(0xFF, 0xFF, activePlayer == 1 ? 0x00 : 0xFF), 240, 220, ALLEGRO_ALIGN_RIGHT, "%s", "Player 2 Level:");

				al_draw_textf(font, al_map_rgb(0xFF, 0xFF, activePlayer == 1 ? 0x00 : 0xFF), 340, 240, ALLEGRO_ALIGN_RIGHT, "%d", layout[1].getLives());
				al_draw_textf(font, al_map_rgb(0xFF, 0xFF, activePlayer == 1 ? 0x00 : 0xFF), 240, 240, ALLEGRO_ALIGN_RIGHT, "%s", "Player 2 Lives:");

				if (!dipSwitches[2]) {
					al_draw_textf(font, al_map_rgb(0xFF, 0xFF, activePlayer == 1 ? 0x00 : 0xFF), 340, 260, ALLEGRO_ALIGN_RIGHT, "%d", layout[1].getShifts());
					al_draw_textf(font, al_map_rgb(0xFF, 0xFF, activePlayer == 1 ? 0x00 : 0xFF), 240, 260, ALLEGRO_ALIGN_RIGHT, "%s", "Player 2 Brick Shifts:");
				}
			}
			else { // NEW: Displays collective scores if in co-op
				al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0x00), 340, 100, ALLEGRO_ALIGN_RIGHT, "%d", layout[0].getScore());
				al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0x00), 240, 100, ALLEGRO_ALIGN_RIGHT, "%s", "Shared Score:");

				al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0x00), 340, 120, ALLEGRO_ALIGN_RIGHT, "%d", layout[0].getLevel());
				al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0x00), 240, 120, ALLEGRO_ALIGN_RIGHT, "%s", "Shared Level:");

				al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0x00), 340, 140, ALLEGRO_ALIGN_RIGHT, "%d", layout[0].getLives());
				al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0x00), 240, 140, ALLEGRO_ALIGN_RIGHT, "%s", "Shared Lives:");
				
				if (!dipSwitches[2]) {
					al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0x00), 340, 160, ALLEGRO_ALIGN_RIGHT, "%d", layout[0].getShifts());
					al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0x00), 240, 160, ALLEGRO_ALIGN_RIGHT, "%s", "Shared Brick Shifts:");
				}
			}
			// Credits
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 340, 400, ALLEGRO_ALIGN_RIGHT, "%s", "Original game makers: Atari");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 340, 420, ALLEGRO_ALIGN_RIGHT, "%s", "Sprite creator used: GraphicsGale");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 340, 440, ALLEGRO_ALIGN_RIGHT, "%s", "Music/SFX maker used: Raster Music Tracker");
			// Text on the right side
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 100, ALLEGRO_ALIGN_LEFT, "%s", "This Breakout clone is made by Bennett");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 120, ALLEGRO_ALIGN_LEFT, "%s", "(Not from Genshin Impact) Johnson.");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 140, ALLEGRO_ALIGN_LEFT, "%s", "Break the bricks to win!");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 160, ALLEGRO_ALIGN_LEFT, "%s", "Version 2.0.0");

			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 200, ALLEGRO_ALIGN_LEFT, "%s", "Arrow keys / WASD: Move paddle");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 220, ALLEGRO_ALIGN_LEFT, "%s", "Right Ctrl / F: Move faster");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 240, ALLEGRO_ALIGN_LEFT, "%s", "Space: Spawn ball");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 260, ALLEGRO_ALIGN_LEFT, "%s", "NUMPAD 1: Start 1 Player Game");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 280, ALLEGRO_ALIGN_LEFT, "%s", "NUMPAD 2: Start 2 Player Game Alternating");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 300, ALLEGRO_ALIGN_LEFT, "%s", "NUMPAD 3: Start 2 Player Game Cooperative");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 320, ALLEGRO_ALIGN_LEFT, "%s", "P: Pause");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 340, ALLEGRO_ALIGN_LEFT, "%s", "Set to:");
			// Game mode display
			switch (players) {
			case 1:
				al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0x00), 1379, 340, ALLEGRO_ALIGN_LEFT, "%s", "1 PLAYER");
				break;
			case 2:
				al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0x00), 1379, 340, ALLEGRO_ALIGN_LEFT, "%s", "2 PLAYER ALT.");
				break;
			case 3:
				al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0x00), 1379, 340, ALLEGRO_ALIGN_LEFT, "%s", "2 PLAYER CO-OP");
				break;
			}
			// Powerup display
			for (POS i = 0, j = 0; j < 10; j++) {
				Powerup::showSprite((WIDTH - HEIGHT) / 2 - Powerup::getWidth(), 460 + j * Powerup::getHeight(), i, j % 2);

				if (j % 2) {
					i++;
				}
			}
			// Powerup explanations
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), (WIDTH - HEIGHT) / 2 - Powerup::getWidth() - 10, 475, ALLEGRO_ALIGN_RIGHT, "%s", "Shrinks your paddle");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), (WIDTH - HEIGHT) / 2 - Powerup::getWidth() - 10, 475 + Powerup::getHeight(), ALLEGRO_ALIGN_RIGHT, "%s", "Makes your paddle longer");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), (WIDTH - HEIGHT) / 2 - Powerup::getWidth() - 10, 475 + Powerup::getHeight() * 2, ALLEGRO_ALIGN_RIGHT, "%s", "Subtracts points");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), (WIDTH - HEIGHT) / 2 - Powerup::getWidth() - 10, 475 + Powerup::getHeight() * 3, ALLEGRO_ALIGN_RIGHT, "%s", "Gives bonus points");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), (WIDTH - HEIGHT) / 2 - Powerup::getWidth() - 10, 475 + Powerup::getHeight() * 4, ALLEGRO_ALIGN_RIGHT, "%s", "Makes the balls faster");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), (WIDTH - HEIGHT) / 2 - Powerup::getWidth() - 10, 475 + Powerup::getHeight() * 5, ALLEGRO_ALIGN_RIGHT, "%s", "Makes the balls slower");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), (WIDTH - HEIGHT) / 2 - Powerup::getWidth() - 10, 475 + Powerup::getHeight() * 6, ALLEGRO_ALIGN_RIGHT, "%s", "MULTIBALL!!!");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), (WIDTH - HEIGHT) / 2 - Powerup::getWidth() - 10, 475 + Powerup::getHeight() * 7, ALLEGRO_ALIGN_RIGHT, "%s", "Makes balls not bounce off bricks");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), (WIDTH - HEIGHT) / 2 - Powerup::getWidth() - 10, 475 + Powerup::getHeight() * 8, ALLEGRO_ALIGN_RIGHT, "%s", "Go up a level");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), (WIDTH - HEIGHT) / 2 - Powerup::getWidth() - 10, 475 + Powerup::getHeight() * 9, ALLEGRO_ALIGN_RIGHT, "%s", "Get an extra life");
			// Showing dipswitch control
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 400, ALLEGRO_ALIGN_LEFT, "%s", "Classic Atari gimmicks are:");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 420, ALLEGRO_ALIGN_LEFT, "%s", "Power up spawning is:");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 440, ALLEGRO_ALIGN_LEFT, "%s", "Game mode is:");

			al_draw_textf(font, al_map_rgb(!dipSwitches[0] ? 0xFF : 0x00, dipSwitches[0] ? 0xFF : 0x00, 0x00), 1509, 400, ALLEGRO_ALIGN_LEFT, "%s", dipSwitches[0] ? "ON" : "OFF");
			al_draw_textf(font, al_map_rgb(!dipSwitches[1] ? 0xFF : 0x00, dipSwitches[1] ? 0xFF : 0x00, 0x00), 1509, 420, ALLEGRO_ALIGN_LEFT, "%s", dipSwitches[1] ? "ON" : "OFF");
			al_draw_textf(font, al_map_rgb(0x00, 0xFF, 0xFF), 1509, 440, ALLEGRO_ALIGN_LEFT, "%s", dipSwitches[2] ? "CLASSIC" : "ENDURANCE");
			// Dipswitch explanations
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 480, ALLEGRO_ALIGN_LEFT, "%s", "NUMPAD 4: Toggle classic gimmicks");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 500, ALLEGRO_ALIGN_LEFT, "%s", "Including paddle shrinking at");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 520, ALLEGRO_ALIGN_LEFT, "%s", "the ceiling, or ball speed at");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 540, ALLEGRO_ALIGN_LEFT, "%s", "certain bricks");

			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 580, ALLEGRO_ALIGN_LEFT, "%s", "NUMPAD 5: Choose whether bricks");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 600, ALLEGRO_ALIGN_LEFT, "%s", "drop powerups or not");

			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 640, ALLEGRO_ALIGN_LEFT, "%s", "NUMPAD 6: Switch modes");

			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 680, ALLEGRO_ALIGN_LEFT, "%s", "All dipswitches are locked in upon");
			al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), 1259, 700, ALLEGRO_ALIGN_LEFT, "%s", "spawning the first ball");
			// Other text
			if (GameObject::isPaused()) {
				al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), WIDTH / 2, 600, ALLEGRO_ALIGN_CENTER, "%s", "Paused");
			}

			if (gameOver()) {
				al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), WIDTH / 2, 600, ALLEGRO_ALIGN_CENTER, "%s", "Game Over");
				al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), WIDTH / 2, 620, ALLEGRO_ALIGN_CENTER, "%s", "Press NUMPAD 1, 2 or 3 to try again.");

				if (message) {
					al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), WIDTH / 2, 500, ALLEGRO_ALIGN_CENTER, "%s", "A new high score!");
					al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), WIDTH / 2, 520, ALLEGRO_ALIGN_CENTER, "%s", "Remember, Mr. Dobervich says to always");
					al_draw_textf(font, al_map_rgb(0xFF, 0xFF, 0xFF), WIDTH / 2, 540, ALLEGRO_ALIGN_CENTER, "%s", DOBERVICH_BUMPER_STICKER);
				}
			}

			for (Paddle& p : paddles) {
				p.draw(); // Display the paddle to the screen
				p.move(); // Make the paddle movable
				p.setXSpeed(0); // Stop the paddle if no key is being held
			}

			for (POS i = 0; i < layout[activePlayer].getBrickSize(); i++) {
				layout[activePlayer].getBrick(i).draw(); // Draw the bricks
			}

			for (Powerup& pu : powerups) {
				pu.draw(); // Draw the power-ups
				pu.move(); // Ensure the power-ups fall down to your paddle
				// Collision detection for powerups against paddles
				for (Paddle& p : paddles) { // Loop over all paddles
					if (pu.getRightSide() >= p.getXPos() && pu.getXPos() <= p.getRightSide() && pu.getBottom() >= p.getYPos() && pu.getBottom() <= p.getYMiddle() + pu.getYSpeed()) {
						al_play_sample(sounds[5], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL); // NEW: This sound byte went previously unused
						// Make things happen when certain power-ups are collected
						switch (pu.getType()) {
						case 0:
							p.addToWidth(pu.getEffect() ? 20 : -20);
							break;
						case 1:
							layout[activePlayer].addToScore(pu.getEffect() ? 1000 * layout[activePlayer].getLevel() : -1000 * layout[activePlayer].getLevel());
							break;
						case 2:
							if (pu.getEffect()) { // Slower
								Ball::addToSpeedCap(3); // Accomodate for going slower
								for (Ball& b : balls) { // Loop through all balls and...
									b.addToXSpeed(b.getXSpeed() > 0 ? -3 : 3);
									b.addToYSpeed(b.getYSpeed() > 0 ? -3 : 3); // ...slow them down
								}
							}
							else { // Faster
								Ball::addToSpeedCap(-3); // Add to the tension by making the balls able to go faster
								for (Ball& b : balls) { // Loop through all balls and...
									b.addToXSpeed(b.getXSpeed() > 0 ? 3 : -3);
									b.addToYSpeed(b.getYSpeed() > 0 ? 3 : -3); // ...speed them up
								}
							}
							break;
						case 3:
							if (pu.getEffect()) { // Fireball
								Ball::setFire(true);
							}
							else { // Multiball
								for (size_t i = 0, j = balls.size() * 2; i < j; i++) {
									balls.push_back(Ball(balls[0]));
								}
							}
							break;
						case 4:
							if (pu.getEffect()) { // 1up
								layout[activePlayer].addToLives(1);
							}
							else { // Level up
								switch (dipSwitches[2]) { // Determines what to do in a mode to constitute a level up
								case 1:
									layout[activePlayer].clearBricks();
									break;
								case 0:
									layout[activePlayer].addToLevel(1);
									break;
								}
							}
							break;
						default:
							cerr << "ERROR: This power-up should not exist. Contact the dev for help!" << endl;
						}

						pu.setYPos(HEIGHT); // Efficient way of destroying powerup objects
					}
				}
			}
			// Key input detector
			if (al_key_down(&key, ALLEGRO_KEY_LEFT)) { // Go left
				paddles[0].setXSpeed(al_key_down(&key, ALLEGRO_KEY_RCTRL) ? -16 : -8);
			}

			if (al_key_down(&key, ALLEGRO_KEY_RIGHT)) { // Go right
				paddles[0].setXSpeed(al_key_down(&key, ALLEGRO_KEY_RCTRL) ? 16 : 8);
			}

			if (al_key_down(&key, ALLEGRO_KEY_A) && players == 3) { // NEW: Go left for Player 2
				paddles[1].setXSpeed(al_key_down(&key, ALLEGRO_KEY_F) ? -16 : -8);
			}

			if (al_key_down(&key, ALLEGRO_KEY_D) && players == 3) { // NEW: Go right for Player 2
				paddles[1].setXSpeed(al_key_down(&key, ALLEGRO_KEY_F) ? 16 : 8);
			}

			if (al_key_down(&key, ALLEGRO_KEY_SPACE) && !inPlay && layout[activePlayer].isActive()) { // Spawn the ball
				dipSwitchLock = true;
				balls.push_back(Ball(WIDTH / 2, 500, 10, 10)); // Spawn the ball
				balls[balls.size() - 1].setXSpeed(rngesus(0, 10) - 5); // Set speeds
				balls[balls.size() - 1].setYSpeed(5);
				inPlay = true;
			}

			if (al_key_down(&key, ALLEGRO_KEY_PAD_1) && !toggleLock) { // If the player wants to start a 1 player game
				dipSwitchLock = false; // NEW: Unlock dipswitch control
				activePlayer = 0;

				if (paddles.size() == 2) { // If there's two paddles
					paddles.pop_back(); // Despawn the top
				}

				Ball::setSpeedCap(-10); // Reset ball speed cap
				layout[0].resetStats(); // Reset stats for Player 1
				layout[1].resetStats(); // Reset stats for Player 2
				// NEW: Determining how to respawn the bricks
				if (dipSwitches[2]) { // If classic
					layout[0].setupBricks(200, (WIDTH - HEIGHT) / 2, 1); // Proceed as planned
				}
				else { // If endurance
					for (short rows = 170, c = 5; rows < 170 + Brick::getHeight() * 6; rows += Brick::getHeight()) {
						if (rows < 170 + Brick::getHeight() * 2 || rows > 170 + Brick::getHeight() * 3) { // Designing the gap
							layout[0].setupBricks(rows, (WIDTH - HEIGHT) / 2, 2, c, false); // For player 1
						}
						else if (c < 6) { // Make sure the color design doesn't overflow
							c++;
						}
					}
				}

				players = 1; // Prevent player switching in game
				balls.clear(); // Clear any left over balls
				powerups.clear(); // Clear any left over powerups
				inPlay = false; // Make the ball spawnable
				message = false;
				// NEW: Unloads power-ups
				Ball::setFire(false); // Resetting powerups
				Ball::setSpeedState(false);
				paddles[0].setWidth(100);
				Paddle::setHits(0);
				Paddle::setShortState(false);
				// NEW: Locks people out of switching on and off as the key is held
				toggleLock = true;
			}

			if (al_key_down(&key, ALLEGRO_KEY_PAD_2) && !toggleLock) { // If the player wants to start a 2 player game
				dipSwitchLock = false;
				activePlayer = 0;

				if (paddles.size() == 2) { // If there's two paddles
					paddles.pop_back(); // Despawn the top
				}

				Ball::setSpeedCap(-10); // Reset ball speed cap
				layout[0].resetStats(); // Reset stats for Player 1
				layout[1].resetStats(); // Reset stats for Player 2
				// Determining how to respawn the bricks
				if (dipSwitches[2]) { // If classic
					layout[0].setupBricks(200, (WIDTH - HEIGHT) / 2, 1); // Proceed as planned
					layout[1].setupBricks(200, (WIDTH - HEIGHT) / 2, 1);
				}
				else { // If endurance
					for (short rows = 170, c = 5; rows < 170 + Brick::getHeight() * 6; rows += Brick::getHeight()) {
						if (rows < 170 + Brick::getHeight() * 2 || rows > 170 + Brick::getHeight() * 3) { // Designing the gap
							layout[0].setupBricks(rows, (WIDTH - HEIGHT) / 2, 2, c, false); // For player 1
							layout[1].setupBricks(rows, (WIDTH - HEIGHT) / 2, 2, c, false); // For player 2
						}
						else if (c < 6) { // Make sure the color design doesn't overflow
							c++;
						}
					}
				}

				players = 2; // Allow player switching each life lost
				balls.clear(); // Clear any left over balls
				powerups.clear(); // Clear any left over powerups
				inPlay = false; // Make the ball spawnable
				message = false;
				
				Ball::setFire(false); // Resetting powerups
				Ball::setSpeedState(false);
				paddles[0].setWidth(100);
				Paddle::setHits(0);
				Paddle::setShortState(false);

				toggleLock = true;
			}
			// Co-op mode
			if (al_key_down(&key, ALLEGRO_KEY_PAD_3) && !toggleLock) {
				dipSwitchLock = false;
				activePlayer = 0;
				// Spawn in a second paddle if need be
				if (paddles.size() < 2) { // If there's only one paddle
					paddles.push_back(Paddle(WIDTH / 2, HEIGHT - 150, WIDTH / 16, 10)); // Spawn another
				}

				Ball::setSpeedCap(-10); // Reset ball speed cap
				layout[0].resetStats(); // Reset stats for Player 1
				// Determining how to respawn the bricks
				if (dipSwitches[2]) { // If classic
					layout[0].setupBricks(200, (WIDTH - HEIGHT) / 2, 1); // Proceed as planned
				}
				else { // If endurance
					for (short rows = 170, c = 5; rows < 170 + Brick::getHeight() * 6; rows += Brick::getHeight()) {
						if (rows < 170 + Brick::getHeight() * 2 || rows > 170 + Brick::getHeight() * 3) { // Designing the gap
							layout[0].setupBricks(rows, (WIDTH - HEIGHT) / 2, 2, c, false); // For player 1
						}
						else if (c < 6) { // Make sure the color design doesn't overflow
							c++;
						}
					}
				}

				players = 3; // Despite the confusing number, allows for 2 paddles at once
				balls.clear(); // Clear any left over balls
				powerups.clear(); // Clear any left over powerups
				inPlay = false; // Make the ball spawnable
				message = false;

				Ball::setFire(false); // Resetting powerups
				Ball::setSpeedState(false);
				paddles[0].setWidth(100);
				paddles[1].setWidth(100);
				Paddle::setHits(0);
				Paddle::setShortState(false);

				toggleLock = true;
			}
			// Dipswitch control
			if (al_key_down(&key, ALLEGRO_KEY_PAD_4) && !toggleLock && !dipSwitchLock) { // Classic Atari gimmicks
				dipSwitches[0] = (dipSwitches[0] ? false : true); // Switch on or off the dipswitch
				toggleLock = true;
			}

			if (al_key_down(&key, ALLEGRO_KEY_PAD_5) && !toggleLock && !dipSwitchLock) { // Power up drops
				dipSwitches[1] = (dipSwitches[1] ? false : true);
				toggleLock = true;
			}

			if (al_key_down(&key, ALLEGRO_KEY_PAD_6) && !toggleLock && !dipSwitchLock) { // Classic VS. Endurance
				dipSwitches[2] = (dipSwitches[2] ? false : true);
				// Switch brick spawn pattern
				layout[0].clearBricks(); // Start by wiping the bricks out of memory to make the new layout
				layout[1].clearBricks();
				// Determining how to respawn the bricks
				if (dipSwitches[2]) { // If classic
					layout[0].setupBricks(200, (WIDTH - HEIGHT) / 2, 1); // Proceed as planned
					layout[1].setupBricks(200, (WIDTH - HEIGHT) / 2, 1);
				}
				else { // If endurance
					for (short rows = 170, c = 5; rows < 170 + Brick::getHeight() * 6; rows += Brick::getHeight()) {
						if (rows < 170 + Brick::getHeight() * 2 || rows > 170 + Brick::getHeight() * 3) { // Designing the gap
							layout[0].setupBricks(rows, (WIDTH - HEIGHT) / 2, 2, c, false); // For player 1
							layout[1].setupBricks(rows, (WIDTH - HEIGHT) / 2, 2, c, false); // For player 2
						}
						else if (c < 6) { // Make sure the color design doesn't overflow
							c++;
						}
					}
				}

				toggleLock = true;
			}
			// Unlock commands
			if (!al_key_down(&key, ALLEGRO_KEY_PAD_1) &&
				!al_key_down(&key, ALLEGRO_KEY_PAD_2) &&
				!al_key_down(&key, ALLEGRO_KEY_PAD_3) &&
				!al_key_down(&key, ALLEGRO_KEY_PAD_4) &&
				!al_key_down(&key, ALLEGRO_KEY_PAD_5) &&
				!al_key_down(&key, ALLEGRO_KEY_PAD_6)) {
				toggleLock = false;
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
					// Shrinking the paddle
					if (dipSwitches[0] && !Paddle::inShortState()) {
						Paddle::setShortState(true);
						// It's an Atari classic feature
						for (Paddle& p : paddles) {
							p.setWidth(50);
						}
					}
				}
				// Paddle collision
				for (Paddle p : paddles) { // Loops over all paddles
					if (b.getRightSide() >= p.getXPos() && b.getXPos() <= p.getRightSide() && b.getBottom() >= p.getYPos() && b.getBottom() <= p.getYMiddle() + b.getYSpeed()) {
						al_play_sample(sounds[0], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
						Paddle::addToHits(1); // NEW: Register a hit for endurance mode

						b.reverseYSpeed(); // Collision detection between ball and paddle
						b.addToXSpeed(p.getXSpeed() / 7.5f + (b.getXMiddle() - p.getXMiddle()) / 5);
						b.addToYSpeed(-0.5);
						b.checkForSpeedCap(); // Make sure the ball doesn't go faster or slower than it needs to
						layout[activePlayer].resetCombo();

						if (dipSwitches[2] && layout[activePlayer].checkForWinner()) { // Check for a winner every time the ball bounces (Classic mode)
							al_play_sample(sounds[4], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
							layout[activePlayer].setupBricks(200, (WIDTH - HEIGHT) / 2, 1);
							layout[activePlayer].addToLevel(1);
							Ball::addToSpeedCap(-2);

							if (layout[activePlayer].getLevel() % 3 == 0) { // NEW: Give a 1up every three levels instead of one
								layout[activePlayer].addToLives(1);
							}
						}
						else if (!dipSwitches[2] && Paddle::getHits() % layout[activePlayer].getSpawnFrequency() == 0) { // Frequency of spawning new bricks
							al_play_sample(sounds[4], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
							layout[activePlayer].shiftBricks();
						}
					}
				}
				// Brick collision
				for (POS i = 0; i < layout[activePlayer].getBrickSize(); i++) { // Loop over bricks
					if (b.getYPos() <= layout[activePlayer].getBrick(i).getBottom() - b.getYSpeed() && b.getBottom() >= layout[activePlayer].getBrick(i).getYPos() - b.getYSpeed()) { // Vertical collision detection
						if (b.getXPos() <= layout[activePlayer].getBrick(i).getRightSide() && b.getRightSide() >= layout[activePlayer].getBrick(i).getXPos()) { // Horizontal
							al_play_sample(sounds[2], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
							// NEW: Speed up
							if (dipSwitches[0] && layout[activePlayer].getBrick(i).isFast() && !Ball::inSpeedState()) {
								Ball::addToSpeedCap(-5);
								b.addToYSpeed(b.getYSpeed() < 0 ? -5 : 5);
								Ball::setSpeedState(true);
							}
							// NEW: Roll for a powerup
							powerRoll = dipSwitches[1] ? rngesus(1, 200) : 1;
							// Which powerup to get
							switch (powerRoll) {
							case 166: // Wide paddle
							case 167: // Or narrow paddle
							case 168: // 1 in 20 chance
							case 169:
							case 170:
							case 171:
							case 172:
							case 173:
							case 174:
							case 175:
								powerups.push_back(Powerup(b.getXPos(), b.getYPos(), Powerup::getWidth(), Powerup::getHeight(), 0, rngesus(0, 1)));
								break;
							case 176: // Bonus points
							case 177: // Or minus points
							case 178: // 1 in 20 chance
							case 179:
							case 180:
							case 181:
							case 182:
							case 183:
							case 184:
							case 185:
								powerups.push_back(Powerup(b.getXPos(), b.getYPos(), Powerup::getWidth(), Powerup::getHeight(), 1, rngesus(0, 1)));
								break;
							case 186:
							case 187: // Speed up
							case 188: // Or speed down
							case 189: // 1 in 20 chance
							case 190:
							case 191:
							case 192:
							case 193:
							case 194:
							case 195:
								powerups.push_back(Powerup(b.getXPos(), b.getYPos(), Powerup::getWidth(), Powerup::getHeight(), 2, rngesus(0, 1)));
								break;
							case 196: // Multiball
							case 197: // Or fireball
							case 198: // 1 in 50 chance
							case 199:
								powerups.push_back(Powerup(b.getXPos(), b.getYPos(), Powerup::getWidth(), Powerup::getHeight(), 3, rngesus(0, 1)));
								break;
							case 200: // Level up or 1up, 1 in 200 chance
								powerups.push_back(Powerup(b.getXPos(), b.getYPos(), Powerup::getWidth(), Powerup::getHeight(), 4, rngesus(0, 1)));
								break;
							}

							layout[activePlayer].addToCombo(); // Add to the combo to award more points
							layout[activePlayer].addToScore(layout[activePlayer].getBrick(i).getPoints() * layout[activePlayer].getCombo() * layout[activePlayer].getLevel()); // Award points
							layout[activePlayer].eraseBrick(i); // Delete the brick that got hit (CALL THIS FUNCTION LAST)
							// NEW: Fireball detection
							if (!Ball::isFire()) { // If the balls aren't fireballs
								b.reverseYSpeed(); // Bounce the ball
							}
						}
					}
				}
			}
			// MODEL YOUR OBJECT DESTROYERS LIKE THIS IN A FOR EACH LOOP, AND KEEP IT OUT OF THE LOOP
			balls.erase( // Shoutouts to Remy Lebeau for this code snippet
				remove_if(balls.begin(), balls.end(),
					[=](Ball& b) {
						// Collision for when you miss
						return b.getYPos() > HEIGHT; // If the ball falls below the defined height of the screen, wipe the ball out of memory to make room
					}
				),
				balls.end()
						);

			powerups.erase(
				remove_if(powerups.begin(), powerups.end(),
					[=](Powerup& pu) {
						return pu.getYPos() > HEIGHT; // Detection for when a power up misses
					}
				),
				powerups.end()
						);
			// Detects if you dropped all the balls
			if (balls.size() == 0 && inPlay) { // Used to crash when a ball is dropped out of reverse order
				layout[activePlayer].addToLives(-1); // Lose a life
				Paddle::setHits(0); // Reset for endurance mode
				inPlay = false; // Make ball spawn available
				// Reset powerup gimmicks
				for (Paddle& p : paddles) {
					p.setWidth(100); // Reset paddle length
					Paddle::setShortState(false);
				}
				
				Ball::setFire(false); // Stop balls from being on fire
				Ball::setSpeedState(false); // Make the balls speed uppable again

				if (!gameOver()) {
					al_play_sample(sounds[3], 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				// Switch turns in alternating multiplayer
				if (layout[(activePlayer + 1) % 2].isActive() && players == 2) { // REMEMBER THE ORDER OF OPERATIONS
					switchTurn();
				}
				// Set the correct ball vertical speed cap for the player
				Ball::setSpeedCap(-8 - 2 * layout[activePlayer].getLevel());
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
	return !layout[0].isActive() && (!layout[1].isActive() || players != 2);
}
// A random number generator named after the god of luck
POS rngesus(POS low, POS high) {
	return rand() % ((high - low) + 1) + low;
}
