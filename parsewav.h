#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct wav_info {
    uint16_t audio_format;   
    uint16_t num_channels;
    uint32_t sample_rate;
    uint16_t bits_per_sample;

    uint32_t data_offset;
    uint32_t data_size;
} wav_info_t;

int read_wav_info(const char *path, wav_info_t *info);
uint16_t read_u16_le(FILE *f);
uint32_t read_u32_le(FILE *f);


