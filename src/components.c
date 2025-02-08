#include "../include/components.h"
#include <stdlib.h>

Button createButton(float x, float y, float w, float h) {
    return (Button) {
        .box = (SDL_FRect){x, y, w, h},
        .clr = (SDL_Color){255, 255, 255, SDL_ALPHA_OPAQUE},
        .hovered = false,
        .pressed = false
    };
}

void renderButton(SDL_Renderer* renderer, Button* but, char* text) {
	SDL_Color c = but->clr;

	if (but->pressed) SDL_SetRenderDrawColor(renderer, c.r - 100, c.g - 100, c.b - 100, c.a - 100);
	else if (but->hovered)	SDL_SetRenderDrawColor(renderer, c.r - 50, c.g - 50, c.b - 50, c.a);
	else SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);

	SDL_RenderFillRect(renderer, &but->box);
	SDL_RenderRect(renderer, &but->box);
}

bool isHovered(SDL_FRect box, float mouseX, float mouseY) {
	return (mouseX >= box.x && mouseX <= box.x + box.w && mouseY >= box.y && mouseY <= box.y + box.h);
}
