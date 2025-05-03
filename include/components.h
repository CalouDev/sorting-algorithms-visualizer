#ifndef COMPONENTS
#define COMPONENTS

#include "globals.h"
#include "utils.h"

typedef struct {
	bool hovered;
	bool pressed;
	SDL_FRect box;
	SDL_Color default_clr;
	SDL_Color hovered_clr;
	SDL_Color pressed_clr;
} Button;

typedef struct {
	Button decrement_btn;
	Button increment_btn;
	size_t value;
	const char* title;
	SDL_FRect main_box;
	SDL_Color bg_clr;
} Counter;

void createButton(Button* btn, SDL_FRect box, SDL_Color default_clr, SDL_Color hovered_clr, SDL_Color pressed_clr);
void renderButton(SDL_Renderer* renderer, const Button* btn);

void createCounter(Counter* counter, size_t val, const char* title, SDL_FRect box, SDL_Color bg_clr, SDL_Color default_clr, SDL_Color hovered_clr, SDL_Color pressed_clr);
void renderCounter(SDL_Renderer* renderer, TTF_TextEngine* text_engine, TTF_Font* font, const Counter* counter);

#endif
