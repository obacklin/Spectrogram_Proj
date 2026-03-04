
#include "parsewav.h"
#include <inttypes.h>

int main(int argc, char* argv[argc+1]){
    
    // Open file
    if (argc < 2){
        fprintf(stderr,"Usage %s <file_name>.wav", argv[0]);
        return EXIT_FAILURE;
    }
    const char * filename = argv[1];
    wav_info_t info;

    read_wav_info(filename, &info);

    printf("Number of channels = %" PRIu16 , info.num_channels);
}