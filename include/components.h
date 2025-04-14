#ifndef COMPONENTS
#define COMPONENTS

#include "globals.h"

typedef struct {
	SDL_FRect box;
	SDL_Color default_clr;
	SDL_Color hovered_clr;
	SDL_Color pressed_clr;
	bool hovered;
	bool pressed;
} Button;

void createButton(Button* btn, SDL_FRect box, SDL_Color default_clr, SDL_Color hovered_clr, SDL_Color pressed_clr);
void renderButton(SDL_Renderer* renderer, const Button* btn);
bool isHovered(SDL_FRect box, float mouse_x, float mouse_y);

#endif
