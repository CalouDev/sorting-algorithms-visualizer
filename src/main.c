#include <stdio.h>
#include <SDL3/SDL.h>
#include "../include/sort_algorithms.h"

/*

   compile with gcc src/main.c -o bin/app -I include -L lib -lmingw32 -lSDL3

*/


#define ARR_SIZE 100 

int main(int argc, char* argv[]) {
	const int version = SDL_GetVersion();

	SDL_Log("SDL %d.%d.%d", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));


    SDL_Window *window;
    bool loop = false;

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(
        "Sorting Algorithms Visualizer",
		640,
		320,
		SDL_WINDOW_OPENGL
	);

    if (window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    while (!loop) {
        SDL_Event event;

		SDL_RenderLine(SDL_Renderer *renderer, 10, 10, 20, 20);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                loop = true;
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
