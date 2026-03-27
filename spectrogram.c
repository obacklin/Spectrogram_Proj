#include "spectrogram.h"
#include <stdlib.h>
#include <complex.h>

int spectrogram_init(spec_t* spec, size_t signal_length, size_t window_size, size_t hop_size){

    spec->window_size = window_size;
    spec->hop_size = hop_size;
    spec->n_frames = 1 + (signal_length - spec->window_size) / spec->hop_size;
    spec->n_bins = spec->window_size / 2 + 1;
    
    if (stft_init(&spec->stft, window_size, hop_size) != 0){
        return -1;
    }
    spec->data = (float *) malloc(spec->n_bins * spec->n_frames * sizeof(float));
    if(!spec->data)
        return -1;
    
    return 0;
}

void spectrogram_compute(spec_t * spec, float * signal){

    // Array for results
    float complex * stft_res = (float complex *) malloc(spec->window_size * sizeof(float complex));
    // Apply stft
    for (size_t i = 0; i < spec->n_frames; i++){
        size_t offset = i*spec->hop_size;
        stft_process_frame(&spec->stft, signal+ offset, stft_res);
        
        for (size_t j = 0; j < spec->n_bins; j++){
            float real = crealf(stft_res[j]);
            float imag = cimagf(stft_res[j]);
            spec->data[i * spec->n_bins + j] = real*real + imag*imag;
        }
    }
    free(stft_res);
    stft_res = NULL;
}

void spectrogram_destroy(spec_t * spec){
    
    stft_destroy(&spec->stft);
    free(spec->data);
    spec->data = NULL;
}