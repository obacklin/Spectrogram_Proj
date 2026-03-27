#include "spectrogram.h"

int spectrogram_init(spec_t* spec, size_t window_size, size_t hop_size){

    spec->window_size = window_size;
    spec->hop_size = hop_size;
    
    if (stft_init(&spec->stft, window_size, hop_size) != 0){
        return -1;
    }

    return 0;
}

void spectrogram_compute(spec_t * spec, float * signal, size_t signal_length){

    // Compute the number of frames
    size_t n_frames = 1 + (signal_length - spec->window_size) / spec->hop_size;
    size_t n_bins = spec->window_size / 2 + 1;

    // Array for results
    float * spectogram = (float *) malloc(n_bins * n_frames * sizeof(float));
    float complex * stft_res = (float complex *) malloc(spec->window_size * sizeof(float complex));
    // Apply stft
    for (size_t i = 0; i < n_frames; i++){
        size_t offset = i*spec->hop_size;

        stft_process_frame(&spec->stft, (const)signal[offset], stft_res);
        for (size_t j = 0; j < n_bins; j++){
            float real = crealf(stft_res[j]);
            float imag = cimagf(stft_res[j]);

            spectogram[i * n_bins + j] = real*real + imag*imag;
        }
    }

    spec->data = spectogram;
    if(stft_res)
        free(stft_res);
    stft_res = NULL;
    
}

void spectrogram_destroy(spec_t * spec){
    if (spec->data)
        free(spec->data);
    spec->data = NULL;
}