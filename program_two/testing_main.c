#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> // used to reference size types
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include "adventure.h"
#include "dir_tests.h"
#include "room_tests.h"

/**
* brief main
*
* return 0
*
* details Details
**/
int main(void){
        srand(time(NULL));
/*     int file_descriptor;
    ssize_t nread;
    ssize_t; */
     const char* dir_name;
    const char* output_file="results.txt";
    gen_files();


    //create_directory();

    
    return 0;
}