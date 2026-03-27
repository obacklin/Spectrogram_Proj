#include "window_stft.h"

void window_hann(float *w, size_t N){
    
    for (size_t n = 0; n < N; n++){
        w[n] = 0.5f * (1- cosf(2* PI_F * n / (N-1)));
    }
}
