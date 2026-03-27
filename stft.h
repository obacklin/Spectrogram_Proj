#ifndef STFT_H
#define STFT_H

#include <stddef.h>
#include <complex.h>
#include "fft_backend.h"

typedef struct stft{
    size_t window_size;
    size_t hop_size;
    float *window;
    float complex *fft_buffer;
    fft_plan_t fft_plan;

}stft_t;

int stft_init(stft_t *stft, size_t window_size, size_t hop_size);

void stft_process_frame(stft_t *stft, const float *input_frame, float complex * output);

void stft_destroy(stft_t *stft);

#endif