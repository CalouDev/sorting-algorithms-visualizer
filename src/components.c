#include "../include/components.h"

Button createButton(float x, float y, float w, float h) {
    return (Button) {
        .box = (SDL_FRect){x, y, w, h},
        .clr = (SDL_Color){255, 255, 255, SDL_ALPHA_OPAQUE},
        .hovered = false,
        .pressed = false
    };
}

void renderButton(SDL_Renderer* renderer, const Button* btn) {
	SDL_Color c = btn->clr;

	if (btn->pressed) SDL_SetRenderDrawColor(renderer, c.r - 100, c.g - 100, c.b - 100, c.a);
	else if (btn->hovered)	SDL_SetRenderDrawColor(renderer, c.r - 50, c.g - 50, c.b - 50, c.a);
	else SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);

	SDL_RenderFillRect(renderer, &btn->box);
	SDL_RenderRect(renderer, &btn->box);
}

bool isHovered(SDL_FRect box, float mouseX, float mouseY) {
	return ((mouseX >= box.x) && (mouseX <= box.x + box.w) && (mouseY >= box.y) && (mouseY <= box.y + box.h));
}

