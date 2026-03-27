#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H
#include "stft.h"
#include <stddef.h>

typedef struct spectrogram {
    size_t window_size;
    size_t hop_size;
    stft_t stft;
    float * data;
    //Flattened data array [frame][bin]
}spec_t;

#endif