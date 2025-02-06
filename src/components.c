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

void renderButton(SDL_Renderer* renderer, Button* but) {
	SDL_Color c = but->clr;
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
	SDL_RenderFillRect(renderer, &but->box);
	SDL_RenderRect(renderer, &but->box);
}
