#include "Powerup.h"

ALLEGRO_BITMAP* Powerup::sprite[5][2];

Powerup::~Powerup() {
	// Destructor
}

void Powerup::draw() {
	al_draw_bitmap(sprite[type][effect ? 1 : 0], xPos, yPos, 0);
}

void Powerup::initializeSprites() {
	sprite[0][0] = al_load_bitmap("Powerdown Paddle size.bmp");
	sprite[0][1] = al_load_bitmap("Powerup Paddle size.bmp");
	sprite[1][0] = al_load_bitmap("Powerdown Points.bmp");
	sprite[1][1] = al_load_bitmap("Powerup Points.bmp");
	sprite[2][0] = al_load_bitmap("Powerdown Speed.bmp");
	sprite[2][1] = al_load_bitmap("Powerup Speed.bmp");
	sprite[3][0] = al_load_bitmap("Powerup Multiball.bmp");
	sprite[3][1] = al_load_bitmap("Powerup Breakthrough.bmp");
	sprite[4][0] = al_load_bitmap("Powerup Level up.bmp");
	sprite[4][1] = al_load_bitmap("Powerup 1up.bmp");
}

void Powerup::showSprite(float x, float y, short t, bool e) {
	al_draw_bitmap(sprite[t][e ? 1 : 0], x, y, 0);
}

short Powerup::getWidth() {
	return al_get_bitmap_width(sprite[0][0]);
}

short Powerup::getHeight() {
	return al_get_bitmap_height(sprite[0][0]);
}

short Powerup::getType() {
	return type;
}

bool Powerup::getEffect() {
	return effect;
}
