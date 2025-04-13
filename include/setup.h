#ifndef SETUP
#define SETUP

#include "globals.h"
#include "sorting_algorithms.h"

static SortData* main_arr = NULL;

static SortingState isSorted = SORTING_CONTINUE;
static SortingState (*sortingFunctions[N_ALGOS])(SortData*) = {sortInsertion, sortSelection, sortBubble, sortMerge};

static Button buttons[N_ALGOS];
static Button incrementDelayButton, decrementDelayButton;

static short greenPassingIndex = -1;

static const char* strSortingFunctions[N_ALGOS] = {"Insertion Sort", "Selection Sort", "Bubble Sort", "Merge Sort"};
static char* fontPath = NULL;
static char* algoTextName = NULL;
static char* strDelayText = NULL;
static char* strSortingInterval = NULL;

static float mouseX, mouseY;

static bool sorting = false;
static bool elementHovered = false;

static uint64_t sortingTimer, greenTimer;

static uint16_t algoChoosen = 0;
static uint16_t sortingInterval = 75;

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

static SDL_MouseButtonFlags mouseData;
static SDL_Cursor* handCursor = NULL;
static SDL_Cursor* defaultCursor = NULL;

static const SDL_Vertex incrementDelayButtonTriangleVertices[3] = {
    {{1240.0f, 312.0f}, FDARK_GREY, FDEFAULT_POINT},
    {{1232.0f, 328.0f}, FDARK_GREY, FDEFAULT_POINT},
    {{1248.0f, 328.0f}, FDARK_GREY, FDEFAULT_POINT}
};

static const SDL_Vertex decrementDelayButtonTriangleVertices[3] = {
    {{1232.0f, 337.0f}, FDARK_GREY, FDEFAULT_POINT},
    {{1248.0f, 337.0f}, FDARK_GREY, FDEFAULT_POINT},
    {{1240.0f, 353.0f}, FDARK_GREY, FDEFAULT_POINT}
};

static TTF_TextEngine* textEngine = NULL;
static TTF_Font* font = NULL;
static TTF_Text* topLeftText = NULL;
static TTF_Text* delayText = NULL;
static TTF_Text* buttonsText[N_ALGOS];

#endif
