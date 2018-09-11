#include "Quadtree.h"

Quadtree::Quadtree(int a_depth, SDL_Rect a_bounds, SDL_Surface* screen) {
	depth = a_depth;
	bounds = a_bounds;
	this->screen = screen;
}

Quadtree::~Quadtree() {
}

void Quadtree::clear() {
	particles.clear();

	for (int i = 0; i < 4; i++) {
		if (quads[i] != nullptr) {
			quads[i]->clear();
			delete quads[i];
			quads[i] = nullptr;
		}
	}
}

void Quadtree::split() {
	int s_width = (int)(bounds.w / 2);
	int s_height = (int)(bounds.h / 2);

	int x = bounds.x;
	int y = bounds.y;

	quads[0] = new Quadtree(depth + 1, { x + s_width, y, s_width, s_height }, screen);
	quads[1] = new Quadtree(depth + 1, { x, y, s_width, s_height }, screen);
	quads[2] = new Quadtree(depth + 1, { x, y + s_height, s_width, s_height }, screen);
	quads[3] = new Quadtree(depth + 1, { x + s_width, y + s_height, s_width, s_height }, screen);
}

const int Quadtree::getIndex(const Particle & a_obj) {
	int index = -1;
	double verticalMiddle = bounds.x + (bounds.w / 2);
	double horizontalMiddle = bounds.y + (bounds.h / 2);

	bool topQuadrant = (a_obj.position.y < horizontalMiddle && a_obj.position.y + 3 < horizontalMiddle);
	bool botQuadrant = (a_obj.position.y > horizontalMiddle);

	if (a_obj.position.x < verticalMiddle && a_obj.position.x + 3 < verticalMiddle) {
		if (topQuadrant) {
			index = 1;
		} else if (botQuadrant) {
			index = 2;
		}
	}

	else if (a_obj.position.x > verticalMiddle) {
		if (topQuadrant) {
			index = 0;
		}
		else if (botQuadrant) {
			index = 3;
		}
	}

	return index;
}

void Quadtree::insert(const Particle & a_obj) {
	if (quads[0] != nullptr) {
		int index = getIndex(a_obj);

		if (index != -1) {
			quads[index]->insert(a_obj);

			return;
		}
	}

	particles.push_back(a_obj);

	if (particles.size() > capacity && depth < max_depth) {
		if (quads[0] == nullptr) {
			split();
		}

		int i = 0;
		while (i < particles.size()) {
			int index = getIndex(particles.at(i));
			if (index != -1) {
				quads[index]->insert(particles.at(i));
				particles.erase(particles.begin() + i);
			}
			else {
				i++;
			}
		}
	}
}

std::vector<Particle*> Quadtree::retrieve(std::vector<Particle*>& returnObjs, Particle & a_obj) {
	int index = getIndex(a_obj);
	if (index != -1 && quads[0] != nullptr) {
		quads[index]->retrieve(returnObjs, a_obj);
	}

	std::vector<Particle*> tmp;
	for (auto p : particles) {
		tmp.push_back(&p);
	}

	returnObjs.insert(returnObjs.end(), tmp.begin(), tmp.end());

	return returnObjs;
}

void Quadtree::Draw() {
	SDL_Rect top, right, bot, left;
	
	top.h = 2;
	top.w = bounds.w;
	top.x = bounds.x;
	top.y = bounds.y;

	right.h = bounds.h;
	right.w = 2;
	right.x = bounds.x + bounds.w;
	right.y = bounds.y;

	bot.h = 2;
	bot.w = bounds.w;
	bot.x = bounds.x;
	bot.y = bounds.y + bounds.h;

	left.h = bounds.h;
	left.w = 2;
	left.x = bounds.x;
	left.y = bounds.y;

	SDL_FillRect(screen, &top, SDL_MapRGB(screen->format, 0, 255, 0));
	SDL_FillRect(screen, &right, SDL_MapRGB(screen->format, 0, 255, 0));
	SDL_FillRect(screen, &bot, SDL_MapRGB(screen->format, 0, 255, 0));
	SDL_FillRect(screen, &left, SDL_MapRGB(screen->format, 0, 255, 0));

	for (int i = 0; i < 4; i++) {
		if (quads[i] != nullptr) {
			quads[i]->Draw();
		}
	}
}
