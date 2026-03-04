/*
    Functions for parsing wav file specs
*/
#include "parsewav.h"

// Little Edian read_healpers

uint16_t read_u16_le(FILE *f) {
    uint8_t b[2];
    fread(b, 1, 2, f);
    return (uint16_t)(b[0] | (b[1] << 8));
}

uint32_t read_u32_le(FILE *f) {
    uint8_t b[4];
    fread(b, 1, 4, f);
    return (uint32_t)(b[0] |
                     (b[1] << 8) |
                     (b[2] << 16) |
                     (b[3] << 24));
}

int read_wav_info(const char *path, wav_info_t *info) {
    
    FILE *file = fopen(path, "rb");
    if (!file){
        return EXIT_FAILURE;
    }

    char id[4];

    //Look for header RIFF
    fread(id, 1, 4, file);
    if (memcmp(id, "RIFF", 4) != 0) {
        fprintf(stderr, "Error: No RIFF header found\n");
        goto fail;
    }

    read_u32_le(file); // File size skip

    fread(id, 1, 4, file);
    if (memcmp(id, "WAVE", 4) != 0) {
        fprintf(stderr, "Error: No WAVE header found\n");
        goto fail;
    }

    int fmt_found = 0;
    int data_found = 0;

    while(!fmt_found || !data_found) {
        if(fread(id,1, 4, file) != 4){
            break;
        }

        uint32_t chunk_size = read_u32_le(file);

        if (memcmp(id, "fmt ", 4) == 0){
            info->audio_format = read_u16_le(file);
            info->num_channels = read_u16_le(file);
            info->sample_rate = read_u32_le(file);
            read_u32_le(file); // byte rate skip
            read_u16_le(file); // block align skip
            info->bits_per_sample = read_u16_le(file);

            if(chunk_size > 16){
                fseek(file, chunk_size - 16, SEEK_CUR);
            }
            
            fmt_found = 1;
        }
        else if (memcmp(id, "data", 4) == 0) {
            info->data_offset = ftell(file);
            info->data_size = chunk_size;
            fseek(file, chunk_size, SEEK_CUR);
            data_found = 1;
        }
        else {
            // Unknown chunk, skip
            fseek(file, chunk_size, SEEK_CUR);
        }

        // Odd chunk size, need to pad to even
        if (chunk_size & 1) {
            fseek(file, 1, SEEK_CUR);
        }
    }

    fail:
        fclose(file);
        return EXIT_FAILURE;

}