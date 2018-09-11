#include "Particle.h"

Particle::Particle(SDL_Surface* renderingSurface, SDL_Point pos) {
	this->renderingSurface = renderingSurface;
	this->position = pos;

	this->velocity = { 1, 1 };
}

Particle::~Particle() {
}

const void Particle::Draw() {
	SDL_Rect rect = { position.x, position.y, 3, 3 };

	SDL_FillRect(renderingSurface, &rect, SDL_MapRGB(renderingSurface->format, 255, 0, 0));
}

void Particle::Tick() {
	int h = renderingSurface->h;
	int w = renderingSurface->w;

	if (position.y > h + 3 || position.y < 0) {
		velocity.y *= -1;
	}

	if (position.x > w + 3 || position.x < 0) {
		velocity.x *= -1;
	}

	position.x += velocity.x;
	position.y += velocity.y;
}
