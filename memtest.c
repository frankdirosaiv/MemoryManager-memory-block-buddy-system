#include "ackerman.h"
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char ** argv) {
    
    //Variables for block size and memsize
    int b = 128;
    int M = 512000;
    
    //Getopt for parsing command line arguments
    if(argc != 5){
        printf("Usage:./testlist [-b <blocksize>] [-s <memsize>]");
        exit(EXIT_FAILURE);
    }
    
    int input = 0;
    while ((input = getopt(argc, argv,"b:s:")) != -1) {
        switch (input) {
            case 'b' : b = atoi(optarg);
                break;
            case 's' : M = atoi(optarg);
                break;
            case '?': printf("Usage:./testlist [-b <blocksize>] [-s <memsize>]");
                exit(EXIT_FAILURE);
        }
    }
    
    //Run functions
    init_allocator(b, M);
    
    ackerman_main();

    //release_allocator();
    atexit(release_allocator());
}
