#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum wav_error {
    WAV_OK = 0,
    WAV_ERR_OPEN,
    WAV_ERR_NOT_RIFF,
    WAV_ERR_NOT_WAVE,
    WAV_ERR_NO_FMT,
    WAV_ERR_NO_DATA
} wav_error_t;

typedef struct wav_error_info {
    wav_error_t err_code;
    int sys_errno;
    const char *path;
} wav_error_info_t;

typedef struct wav_info {
    uint16_t audio_format;   
    uint16_t num_channels;
    uint32_t sample_rate;
    uint16_t bits_per_sample;

    uint32_t data_offset;
    uint32_t data_size;
} wav_info_t;

const char *wav_strerror(wav_error_t err);
wav_error_t read_wav_info(const char *path, wav_info_t *info, wav_error_info_t * errinfo);
