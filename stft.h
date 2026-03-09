#ifndef STFT_H
#define STFT_H
#include <complex.h>
#include "fft_backend.h"
#include "window_stft.h"

typedef struct stft{
    int window_size;
    int hop_size;
    int fft_size;
    float *window;
    float *frame_buffer;
    float complex *fft_buffer;
    fft_plan_t fft_plan;

}stft_t;

int stft_init( stft_t *stft,int window_size, int hop_size, int fft_size);

void stft_process_frame(stft_t *stft, const float *input, float complex * output);

void stft_destroy(stft_t *stft);

#endif