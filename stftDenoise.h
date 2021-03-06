#ifndef AUDIO_DENOISE_H_
#define AUDIO_DENOISE_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "kiss_fftr.h"

#define OK                 0x00
#define ERROR_MEMORY       0x01
#define ERROR_PARAMS       0x02
#define NEED_MORE_SAMPLES  0x10
#define CAN_OUTPUT         0x20

typedef struct StftDenoiseHandle {
    int32_t win_size;    // window size--odd window
    int32_t half_win_size; // half window size
    float *win_hanning; // hanning window

    int32_t max_nblk_time;
    int32_t max_nblk_freq;
    int32_t nblk_time;  // the number of block in time dimension
    int32_t nblk_freq;  // the number of block in frequency dimension
    int32_t macro_size; // the number of sample in one macro block
    int32_t have_nblk_time;
    float **SURE_matrix;

    float sigma_noise;  // assumption the sigma of gaussian white noise
    float sigma_hanning_noise;
    kiss_fft_scalar *inbuf;       // internal buffer for keep one window size input samples
    kiss_fft_scalar *inbuf_win;
    kiss_fft_scalar *outbuf;      // internal buffer for keep one macro block output samples

    kiss_fft_cpx **stft_coef;
    kiss_fft_cpx **stft_thre;
    kiss_fft_cpx **stft_coef_block;
    kiss_fft_cpx **stft_coef_block_norm;
    kiss_fftr_cfg forward_fftr_cfg;
    kiss_fftr_cfg backward_fftr_cfg;
} stftDenoiseHandle;

stftDenoiseHandle *stftDenoise_init(int32_t time_win, int32_t fs, int32_t *err, float sigma_noise);

int32_t stftDenoise_reset(stftDenoiseHandle *handle);

int32_t stftDenoise_denoise_scalar(stftDenoiseHandle *handle, kiss_fft_scalar *in, int32_t in_len);

int32_t stftDenoise_output_scalar(stftDenoiseHandle *handle, kiss_fft_scalar *out, int32_t out_len);

int32_t stftDenoise_flush_scalar(stftDenoiseHandle *handle, kiss_fft_scalar *out, int32_t out_len);

void stftDenoise_free(stftDenoiseHandle *handle);

int32_t stftDenoise_max_output(const stftDenoiseHandle *handle);

int32_t stftDenoise_samples_per_time(const stftDenoiseHandle *handle);

#endif