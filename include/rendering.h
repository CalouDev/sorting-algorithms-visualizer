#ifndef RENDERING
#define RENDERING

#include "globals.h"
#include "setup.h"

void clearRenderer(SDL_Color bg_clr);
void renderText(void);
void renderComponents(void);
void renderAndProcessSortingEffect(void);
void renderGreenEffect(void);
void incrementGreenEffect(void);

#endif
