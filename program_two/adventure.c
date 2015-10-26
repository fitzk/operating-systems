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
#include "adventure.h"
// Thread-safe usage of strerror_r().
//http://www.ibm.com/developerworks/aix/library/au-errnovariable/
void thread_safe( int err )
{
    char buff[256];
    
    if( strerror_r( err, buff, 256 ) == 0 ) {
        printf( "Error: %s\n", buff );
    }
}
int create_directory(){
    
    int pid=getpid();
    char dir_name[80];
    const char* dir_name_ptr;
    DIR *dir_ptr;
   
    
   sprintf(dir_name,"fitzsimk.rooms.%d",pid);
    dir_name_ptr=dir_name;
   if( mkdir( dir_name_ptr,0755) != 0) {

   }           
    
      
    

    return 0;
} 


