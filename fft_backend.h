#ifndef FFT_BACKEND_H
#define FFT_BACKEND_H
#include <complex.h>
#include <stddef.h>
#include "spec_constants.h"

typedef struct fft_plan {

    size_t N;
    size_t log2N;
    size_t *bitrev;
    float complex *twiddles;

} fft_plan_t;

void fft_execute(const fft_plan_t *plan, float complex *x);
int fft_init(fft_plan_t *plan, size_t N);
void fft_plan_destroy(fft_plan_t *plan);

#endif