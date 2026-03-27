#include "stft.h"
#include "window_stft.h"
#include <stdlib.h>
#include <stddef.h>


int stft_init(stft_t *stft, size_t window_size, size_t hop_size){
    /*
        Sets up the struct, allocates and makes fft plan.
    */
    stft->window_size = window_size;
    stft->hop_size = hop_size;

    float * win = (float*) malloc(window_size * sizeof(float));
    stft->window = win;
    window_hann(win, window_size);

    if (fft_init(&stft->fft_plan, window_size) != 0){
        return -1;
    }
    float complex * fft_buffer = (float complex *) malloc(window_size * sizeof(float complex));
    stft->fft_buffer = fft_buffer;

    if(!stft->window || !stft->fft_buffer){
        return -1;
    }
    return 0;
}

void stft_process_frame(stft_t *stft, const float *input_frame, float complex * output){
    
    size_t N = stft->window_size;

    // Convert to complex
    for (size_t i = 0; i < N; i++){
        stft->fft_buffer[i] = input_frame[i] * stft->window[i] + 0.0f*I;
    }
    // Transform frame
    fft_execute(&stft->fft_plan, stft->fft_buffer);
    // Copy to output
    for (size_t i = 0; i < N; i++){
        output[i] = stft->fft_buffer[i];
    }
}

void stft_destroy(stft_t * stft){

    free(stft->window);
    free(stft->fft_buffer);
    fft_plan_destroy(&stft->fft_plan);
    stft->fft_buffer = NULL;
    stft->window = NULL;
}