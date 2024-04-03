#include <stdio.h>
#include <SDL2/SDL_audio.h>
#include <stdlib.h>
#include "header.h"

extern double *buffer_fake;
SDL_AudioDeviceID dev;

void data_callback(void *userdata, Uint8 *stream, int lent){
    (void)userdata;
    float *in = (float*)stream;
    int len = lent/4;

    /* shift */
    for (int i=0; i<buffer_size-len; ++i) {
        buffer_fake[i] = buffer_fake[i+len];
    }
    /* add new */
    for (int i=0; i < len; i+=1) {
        buffer_fake[i+buffer_size-len] = in[i];
    }
    algo();
}

int audio_init(void){
    if (SDL_AudioInit(NULL) != 0) {
        fprintf(stderr, "SDL_AudioInit error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_AudioSpec want, have;

    /* open output device */
    SDL_zero(want);
    want.freq     = sample_rate;
    want.format   = AUDIO_F32SYS;
    want.channels = 1;
    want.samples  = frames_per_buffer;
    want.callback = data_callback;
    dev = SDL_OpenAudioDevice(NULL, 1, &want, &have, 0);

    if (!dev) {
        fprintf(stderr, "failed to open recording device: %s\n", SDL_GetError());
        return 1;
    }

    SDL_PauseAudioDevice(dev, 0);
    return 0;
}

void audio_terminate(void){
    SDL_CloseAudioDevice(dev);
}
