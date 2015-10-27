/**
* author: Kayla Fitzsimmons
* course: Operating Systems
* term: Fall 2015
* program: 2
* file: fitzsimk.adventure.c
* description: File based game written in c
* ...
**/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> // used to reference size types
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include<string.h>
#include "adventure.h"



int gen_files(){
    srand(time(NULL));
    
    // create null pointer to struct type name_option
    struct name_option *names= NULL;
    define_file_names(&names);

}
int define_file_names(struct name_option ** names){
    int count=0;
    // array of potential file names
    const char* ch_names[]={"a","b","c","d","e","f","g","h","i","j"};
   
    // allocating memory to create 10 name_option structs
    *names= (struct name_option*)malloc(sizeof(*names)*100);
    int k;
    for(k=0; k < 10;k++){
        printf("ch_names: %s\n",ch_names[k]);
        if ( strncpy(names[k]->name,ch_names[k],20) )
        {
             printf("names[k]->name: %s\n",names[k]->name);
        }
      //  
    //    names[k]->chosen=0;
    }

    int r = 8;

/*     while(count < 7){
        r = rand()%7;
 //       if()
        
        printf("names[0]->name: %s\n",names[0]->name);
    
    }
 */
 return 0;
}
// Thread-safe usage of strerror_r().
//http://www.ibm.com/developerworks/aix/library/au-errnovariable/
void thread_safe( int err )
{
    char buff[256];
    
    if( strerror_r( err, buff, 256 ) == 0 ) {
        printf( "Error: %s\n", buff );
    }
}
/**
*  create_directory
*
*  return: 0 on success
*
*  details: Creates a directory in the format fitzsimk.rooms.<current pid>
*               initial file permissions set to 755
**/
int create_directory(){
    
    int pid=getpid();
    char dir_name[80];
    const char* dir_name_ptr;
    DIR *dir_ptr;
    
   sprintf(dir_name,"fitzsimk.rooms.%d",pid);
    dir_name_ptr=dir_name;
   if( mkdir( dir_name_ptr,0755) != 0) {
            printf("Add error checking and output to stderr");
   }           
    return 0;
} 


