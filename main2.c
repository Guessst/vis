#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "kiss_fft.h"

#define N 128  // FFT size (must be power of 2)
#define M_PI 3.14159265358979323846
#define SAMPLE_RATE 48000.0f
#define FREQ 375.0f

int main(void) {
    kiss_fft_cfg cfg = kiss_fft_alloc(N, 0, NULL, NULL);
    if (!cfg) {
        fprintf(stderr, "Failed to allocate KissFFT config\n");
        return 1;
    }

    kiss_fft_cpx in[N];
    kiss_fft_cpx out[N];

    // Generate float32 samples (sine wave)
    for (int i = 0; i < N; i++) {
        float s = sinf(2.0f * M_PI * FREQ * i / SAMPLE_RATE);
        in[i].r = s;
        in[i].i = 0.0f;
    }

    kiss_fft(cfg, in, out);

    // Print first 10 bins of magnitude
    for (int i = 0; i < N; i++) {
        float mag = sqrtf(out[i].r * out[i].r + out[i].i * out[i].i);
        printf("Bin %2d: %.4f\n", i, mag);
    }

    free(cfg);
    return 0;
}
