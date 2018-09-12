#pragma once

#include <vector>
#include "Particle.h"

class Quadtree
{
public:
	Quadtree(int a_depth, SDL_Rect a_bounds, SDL_Surface* screen);
	~Quadtree();

	static constexpr int capacity = 10;
	static constexpr int max_depth = 10;

	SDL_Surface* screen;
	int depth;
	std::vector<Particle*> particles;
	SDL_Rect bounds;
	Quadtree* quads[4]{ nullptr };

	void clear();
	void split();
	int getIndex(Particle* a_obj);
	void insert(Particle* a_obj);
	std::vector<Particle*> retrieve(std::vector<Particle*>& returnObjs, Particle* a_obj);
	void getSingleFromAllQuads(std::vector<Particle*>& particles);
	void Draw();
};

