//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <vector>

#include "Particle.h"
#include "Quadtree.h"

//Screen dimension constants
const int SCREEN_WIDTH = 860;
const int SCREEN_HEIGHT = 600;

SDL_Window* gWindow = nullptr;

SDL_Surface* gScreenSurface = nullptr;

//start SDL and open window
bool init() {

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	//Create window
	gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == nullptr)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	//Get window surface
	gScreenSurface = SDL_GetWindowSurface(gWindow);

	return true;
}

//Frees media and close SDL
void close() {
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;

	SDL_Quit();
}

int main(int argc, char* args[])
{
	if (!init()) {
		close(); 

		return 0;
	}

	bool exit = false;
	SDL_Event e;
	

	std::vector<Particle> particles;
	Quadtree quad(0, { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }, gScreenSurface);
	
	while (!exit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				exit = true;
				continue;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				int x, y;
				SDL_GetMouseState(&x, &y);

				particles.push_back(Particle(gScreenSurface, { x, y }));

				continue;
			}
		}

		SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 255, 255, 255));

		quad.clear();
		for (auto & p : particles) {
			quad.insert(p);
			
			std::vector<Particle*> returnObjs;
			quad.retrieve(returnObjs, p);

			if (returnObjs.size() > 1) {

				for (int i = 0; i < returnObjs.size(); i++) {
					for (int j = 0; j < returnObjs.size(); j++) {
						if (j == i) {
							continue;
						}
						Particle* p = returnObjs[i];
						Particle* r_p = returnObjs[j];
						if (p->position.x + 3 >= r_p->position.x && p->position.x < r_p->position.x + 3 && p->position.y + 3 > r_p->position.y && p->position.y < r_p->position.y + 3) {
							p->velocity.x *= -1;
							p->velocity.y *= -1;
						}
					}
				}
			}

			p.Tick();
			p.Draw();
		}
		
		quad.Draw();
		
		SDL_UpdateWindowSurface(gWindow);

		SDL_Delay(5);
	}

	close();

	return 0;
}