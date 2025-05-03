#include "../include/sound.h"

void playSound(SDL_AudioStream* stream, int freq, int* sine) {
	const int minimum_audio = (2000 * sizeof (float));

    if (SDL_GetAudioStreamQueued(stream) < minimum_audio) {
        static float samples[1024];
        size_t i;

        for (i = 0; i < SDL_arraysize(samples); ++i) {
            const float phase = (*sine) * freq / 8000.0f;
            samples[i] = SDL_sinf(phase * 2 * SDL_PI_F);
            (*sine)++;
        }

        (*sine) %= 8000;

        SDL_PutAudioStreamData(stream, samples, sizeof (samples));
    }
}
