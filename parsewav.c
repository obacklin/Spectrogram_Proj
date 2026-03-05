#include "parsewav.h"
/*
    Functions for parsing wav file specs
*/

// Little Edian read healpers
static uint16_t read_u16_le(FILE *f) {
    uint8_t b[2];
    fread(b, 1, 2, f);
    return (uint16_t)(b[0] | (b[1] << 8));
}

static uint32_t read_u32_le(FILE *f) {
    uint8_t b[4];
    fread(b, 1, 4, f);
    return (uint32_t)(b[0] |
                     (b[1] << 8) |
                     (b[2] << 16) |
                     (b[3] << 24));
}

static inline void wav_set_error(wav_error_info_t *errinfo, wav_error_t err_code, const char * path){
    if(!errinfo)
        return;
    errinfo->err_code = err_code;
    errinfo->sys_errno = errno;
    errinfo->path = path;
}

static const char *wav_error_strings[] = {
    [WAV_OK]  = "No error",
    [WAV_ERR_OPEN] = "Falied to open file",
    [WAV_ERR_NOT_RIFF] = "No RIFF header found",
    [WAV_ERR_NOT_WAVE] = "Not a WAVE file",
    [WAV_ERR_NO_FMT] = "Chunk fmt not found",
    [WAV_ERR_NO_DATA] = "Chunk data not found"
};

const char *wav_strerror(wav_error_t err){
    if (err < 0 || err >= (int)(sizeof(wav_error_strings)/sizeof(*wav_error_strings))){
        return "Unknown WAV error";
    }
    
    return wav_error_strings[err];
}

wav_error_t read_wav_info(const char *path, wav_info_t *info, wav_error_info_t * errinfo){
    
    wav_error_t result = WAV_OK;
    FILE *file = NULL;
    
    file = fopen(path, "rb");
    if (!file){
        result =  WAV_ERR_OPEN;
        wav_set_error(errinfo, result, path);
        goto cleanup;
    }

    char id[4];

    //Look for header RIFF
    fread(id, 1, 4, file);
    if (memcmp(id, "RIFF", 4) != 0) {

        result = WAV_ERR_NOT_RIFF;
        goto cleanup;
    }

    read_u32_le(file); // File size skip

    fread(id, 1, 4, file);
    if (memcmp(id, "WAVE", 4) != 0) {
        result = WAV_ERR_NOT_WAVE;
        wav_set_error(errinfo, result, path);
        goto cleanup;
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

    if(!fmt_found){
        result = WAV_ERR_NO_FMT;
        wav_set_error(errinfo, result, path);
        goto cleanup;
    }

    if(!data_found){
        result = WAV_ERR_NO_DATA;
        wav_set_error(errinfo, result, path);
        goto cleanup;
    }


    cleanup:
    if (file)
        fclose(file);

    return result;

}

void stream_data(const char *path, wav_info_t *info, float *arr){

}