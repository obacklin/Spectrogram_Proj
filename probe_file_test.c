#include "parsewav.h"
#include <inttypes.h>

int main(int argc, char* argv[argc+1]){
    
    if (argc < 2){
        fprintf(stderr, "Usage %s <file_name>.wav", argv[0]);
        return EXIT_FAILURE;
    }
    const char * filename = argv[1];
    wav_info_t info;
    wav_error_info_t errinfo = {0};

    wav_error_t err = read_wav_info(filename, &info, &errinfo);
    
    if(err) {
        fprintf(stderr,
            "Error code [%d] in '%s': %s (%s)\n", 
            errinfo.err_code,
            errinfo.path,
            wav_strerror(errinfo.err_code),
            strerror(errinfo.sys_errno));
        
        return EXIT_FAILURE;
    }
    
    printf("Number of channels = %" PRIu16 , info.num_channels);

    return EXIT_SUCCESS;
}