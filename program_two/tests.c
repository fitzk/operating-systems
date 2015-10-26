/**
*  author: Kayla Fitzsimmons
*  course: Operating Systems
*  term: Fall 2015
*  program: 2
*  file: tests.c
*  description: Unit tests for adventure.c
*  ...
**/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> // used to reference size types
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include "adventure.h"


/**
*  directory_test2
*
*  param: [in] dir_name    name of the directory
*  param: [in] output_file   name of file to send results to
*  return: returns 1 if it could not open the directory, and 0 on success 
*
*  details: tests if directory exits
**/
int directory_test2(const char* dir_name,const char* output_file){
        FILE* fp;
        int pid;
        fp= fopen(output_file,"w+");
        char expected_name[256]; 
        int result;
        // get process id
        pid=getpid();
        
        // store expected name to c
        sprintf(expected_name,"fitzsimk.rooms.%d",pid);
        fprintf (fp,"Test 2)\n Testing that directory is named correctly...\n");
        fprintf(fp,"- Expected: %s\n- Actual: %s",expected_name,dir_name);
        fclose(fp);
        return 0;
    
}
/**
*  directory_test1
*
*  param: [in] dir_name    name of the directory
*  param: [in] output_file   name of file to send results to
*  return: returns 1 if it could not open the directory, and 0 on success 
*
*  details: tests if directory exits
**/
int directory_test1(const char* dir_name,const char* output_file){
        FILE* fp;
        DIR *p_dir;
        struct dirent *p_dirent;
        fp= fopen(output_file,"w+");
        p_dir = opendir (dir_name);
        
        fprintf (fp,"Test 1)\n Testing for directory '%s'\n", dir_name);        
        if (p_dir == NULL) {
            fprintf (fp,"Cannot open directory '%s'\n", dir_name);
            return 1;
        }

        while ((p_dirent = readdir(p_dir)) != NULL) {
            fprintf (fp,"[%s]\n", p_dirent->d_name);
        }
        closedir (p_dir);
        fclose(fp);
        return 0;
    
}
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
    
   //const char* dir_name;
   const char* output_file="results.txt";

    
    ///dir_name= 
    create_directory();
    //int i =directory_test1(dir_name,output_file);
   // if(i==0)
     //   directory_test2(dir_name,output_file);
    
    return 0;
}