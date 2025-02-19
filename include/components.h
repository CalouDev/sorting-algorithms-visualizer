#ifndef COMPONENTS
#define COMPONENTS

#include <SDL3/SDL.h>

typedef struct {
	SDL_FRect box;
	SDL_Color clr;
	bool hovered;
	bool pressed;
} Button;

Button createButton(float x, float y, float w, float h);
void renderButton(SDL_Renderer* renderer, Button* but);
bool isHovered(SDL_FRect box, float mouseX, float mouseY);

#endif
