#include "../include/components.h"

void createButton(Button* btn, SDL_FRect box, SDL_Color default_clr, SDL_Color hovered_clr, SDL_Color pressed_clr) {
    btn->box = box;
	btn->default_clr = default_clr;
	btn->hovered_clr = hovered_clr;
	btn->pressed_clr = pressed_clr;
	btn->hovered = false;
	btn->pressed = false;
}

void renderButton(SDL_Renderer* renderer, const Button* btn) {
	SDL_Color current_clr;

	current_clr = btn->default_clr;

	if (btn->pressed) current_clr = btn->pressed_clr;
	else if (btn->hovered)	current_clr = btn->hovered_clr;
	
	SDL_SetRenderDrawColor(renderer, current_clr.r, current_clr.g, current_clr.b, current_clr.a);

	SDL_RenderFillRect(renderer, &btn->box);
	SDL_RenderRect(renderer, &btn->box);
}

bool isHovered(SDL_FRect box, float mouse_x, float mouse_y) {
	return ((mouse_x >= box.x) && (mouse_y <= box.x + box.w) && (mouse_y >= box.y) && (mouse_y <= box.y + box.h));
}

