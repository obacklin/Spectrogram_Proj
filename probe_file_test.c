
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[argc+1]){
    
    // Open file
    if (argc < 2){
        fprintf(stderr,"Usage %s <file_name>.wav", argv[0]);
        return EXIT_FAILURE;
    }

    FILE* file = fopen(argv[1], "rb");
    if (!file){
        return EXIT_FAILURE;
    }

    

    fclose(file);
    return EXIT_SUCCESS;
}