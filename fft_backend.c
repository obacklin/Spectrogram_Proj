#include "fft_backend.h"
#include <stdlib.h>
#include <math.h>

static size_t reverse_bits(size_t x, size_t bits)
{
    size_t r = 0;

    for (size_t i = 0; i < bits; i++)
    {
        r <<= 1;
        r |= (x & 1);
        x >>= 1;
    }

    return r;
}

static void precompute_bitrev(fft_plan_t *plan)
{
    size_t N = plan->N;
    size_t bits = plan->log2N;

    for (size_t i = 0; i < N; i++)
        plan->bitrev[i] = reverse_bits(i, bits);
}

static void precompute_twiddles(fft_plan_t *plan)
{
    size_t N = plan->N;

    for (size_t k = 0; k < N/2; k++)
    {
        float angle = -2.0f * PI_F * (float)k / (float)N;
        plan->twiddles[k] = cexpf(I * angle);
    }
}

static size_t compute_log2(size_t N)
{
    size_t log2N = 0;

    while ((size_t)1 << log2N < N)
        log2N++;

    return log2N;
}

int fft_init(fft_plan_t *plan, size_t N){
    if ((N & (N-1)) != 0){
        return -1;
    }
    plan->N = N;
    plan->log2N = compute_log2(N);

    plan->bitrev = malloc(N * sizeof(size_t));
    if(!plan->bitrev){
        return -1;
    }
    
    plan->twiddles = malloc((N/2) * sizeof(float complex));
    if(!plan->twiddles){
        free(plan->bitrev);
        return -1;
    }

    precompute_bitrev(plan);
    precompute_twiddles(plan);

    return 0;

}

void fft_plan_destroy(fft_plan_t *plan)
{   
    free(plan->bitrev);
    free(plan->twiddles);
    plan->bitrev = NULL;
    plan->twiddles = NULL;
}

void fft_execute(const fft_plan_t *plan, float complex *x)
{
    size_t N = plan->N;

    for (size_t i = 0; i < N; i++)
    {
        size_t j = plan->bitrev[i];

        if (i < j)
        {
            float complex tmp = x[i];
            x[i] = x[j];
            x[j] = tmp;
        }
    }

    for (size_t m = 2; m <= N; m <<= 1)
    {
        size_t half = m >> 1;
        size_t step = N / m;

        for (size_t k = 0; k < N; k += m)
        {
            for (size_t j = 0; j < half; j++)
            {
                float complex w = plan->twiddles[j * step];
                float complex t = w * x[k + j + half];
                float complex u = x[k + j];

                x[k + j]        = u + t;
                x[k + j + half] = u - t;
            }
        }
    }
}