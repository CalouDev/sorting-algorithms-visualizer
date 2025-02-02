#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <pthread.h>
#include "../include/sorting_algorithms.h"


/*

   compile with gcc src/main.c src/sorting_algorithms.c -o bin/app -I include -L lib -lmingw30 -lSDL3 

*/

#define WIN_W 1280
#define WIN_H 640

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

ushort arr[10] = {4, 8, 3, 5, 10, 7, 2, 9, 1, 6};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Error initializing SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    window = SDL_CreateWindow("Sorting Algorithms Visualizer", WIN_W, WIN_H, SDL_WINDOW_OPENGL);

    if (!window) {
        SDL_Log("Error creating window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    renderer = SDL_CreateRenderer(window, NULL);

    if (!renderer) {
        SDL_Log("Error creating renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }	

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    } else if (event->type == SDL_EVENT_KEY_DOWN) {
		if (event->key.scancode == SDL_SCANCODE_SPACE) {
			sortInsertion(arr, 10);
		}
	}
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
	
	for (int i = 0; i < 10; ++i) {
		SDL_RenderLine(renderer, 10 + i * 10, WIN_H, 10 + i * 10, WIN_H - arr[i] * 60);
	}

	SDL_RenderPresent(renderer);

	return SDL_APP_CONTINUE;
}
