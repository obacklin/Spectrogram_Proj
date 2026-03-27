#include "parsewav.h"
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "spectrogram.h"

int main(int argc, char* argv[argc+1]){
    
    if (argc < 2){
        fprintf(stderr, "Usage %s <file_name>.wav", argv[0]);
        return EXIT_FAILURE;
    }
    const char * filename = argv[1];
    wav_info_t info;
    wav_error_info_t errinfo = {0};
    errinfo.path = filename;

    FILE *file = fopen(filename, "rb");
    if (!file){
        errinfo.err_code = WAV_ERR_OPEN;
        fprintf(stderr,
            "Error code [%d] in '%s': %s (%s)\n", 
            errinfo.err_code,
            errinfo.path,
            wav_strerror(errinfo.err_code),
            strerror(errinfo.sys_errno));
            return EXIT_FAILURE;
    }

    wav_error_t err = read_wav_info(file, &info, &errinfo);
    
    if(err) {
        fprintf(stderr,
            "Error code [%d] in '%s': %s (%s)\n", 
            errinfo.err_code,
            errinfo.path,
            wav_strerror(errinfo.err_code),
            strerror(errinfo.sys_errno));
            goto cleanup;
    }

    float * pcm_float;
    pcm_float = wav_load_frames(file, &info);

    spec_t spectrogram;
    spectrogram_init(&spectrogram, info.n_frames, 1024, 128);
    spectrogram_compute(&spectrogram, pcm_float);

    //Test print 
    for (size_t i = 0; i < 100; i++){
        printf("Frame %zu: ", i);
        for (size_t j = 0; (j < 10) && (j < spectrogram.n_bins); j++ ){
            printf("%f ", spectrogram.data[i*spectrogram.n_bins + j]);
        }
        printf("\n");
    }


    return EXIT_SUCCESS;
    cleanup:
    fclose(file);
    return EXIT_FAILURE;
}