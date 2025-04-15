#ifndef GLOBALS
#define GLOBALS

#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <time.h>

#define WIN_W 1280U
#define WIN_H 640U
#define GREEN_EFFECT_INTERVAL 2U
#define ARR_SIZE 250 // U
#define N_ALGOS 4U
#define DELAY_MAX_LIM_N 4U
#define MIN_DELAY 25
#define MAX_DELAY 975
#define STEP_DELAY 25

typedef enum {
    SORTING_STOP,
    SORTING_CONTINUE
} SortingState;

typedef struct {
    uint16_t size;
    uint16_t index;
    short arr[];
} SortData;

static const SDL_Color CLR_WHITE = {255, 255, 255, 255};
static const SDL_Color CLR_LIGHT_GREY = {205, 205, 205, 255};
static const SDL_Color CLR_GREY = {155, 155, 155, 255};
static const SDL_FColor FDARK_GREY = {0.4f, 0.4f, 0.4f, 1.0f};
static const SDL_Color CLR_GREEN = {0, 255, 0, 255};
static const SDL_Color CLR_RED = {255, 0, 0, 255};
static const SDL_Color CLR_BLACK = {10, 10, 10, 255};
static const SDL_FPoint FDEFAULT_POINT = {0.0f, 0.0f};

#endif
