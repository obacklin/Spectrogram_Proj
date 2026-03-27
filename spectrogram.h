#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H
#include "stft.h"
#include <stddef.h>

typedef struct spectrogram {
    size_t window_size;
    size_t hop_size;
    size_t n_frames;
    size_t n_bins;
    stft_t stft;
    float * data;
    //Flattened data array [frame][bin]
}spec_t;


int spectrogram_init(spec_t* spec, size_t signal_length, size_t window_size, size_t hop_size);
void spectrogram_compute(spec_t * spec, float * signal);
void spectrogram_destroy(spec_t * spec);


#endif