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
    
/*     int file_descriptor;
    ssize_t nread;
    ssize_t; */
          const char* dir_name;
    const char* output_file="results.txt";
    gen_files();

    ///dir_name= 
    create_directory();
    //int i =directory_test1(dir_name,output_file);
   //  if(i==0)
     //   directory_test2(dir_name,output_file);
    
    return 0;
}