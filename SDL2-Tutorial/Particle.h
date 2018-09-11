#pragma once

#include <SDL_surface.h>

class Particle
{
public:
	Particle(SDL_Surface* renderingSurface, SDL_Point pos);
	~Particle();

	const void Draw();
	void Tick();

	SDL_Point position;
	SDL_Point velocity;
	SDL_Surface* renderingSurface;
};

