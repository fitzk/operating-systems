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

    // create10 name_option structs to hold naming schema
    struct name_option names[10];
    // create 7 room structs to hold room schema
    struct room* rooms[7];    
    // store potential file names in structs
    define_file_names(&names);
    name_rooms(&names, &rooms);
    set_connections(&names,&rooms);
}
/**
* define_file_names 
*
*  param: names 
*  return: 0
*
*  details: sets name member in all name_option structs, and sets chosen flag to 0
**/
int define_file_names(struct name_option* names){

    // array of potential file names
    const char* ch_names[]={"aah\0","bah\0","cah\0","dah\0","ehh\0","fah\0","gah\0","hha\0","iah\0","jah\0"};
    
    // assigning names to structs 
    int k;
    for(k=0; k < 10;k++){
        strcpy(names[k].name, ch_names[k]);
         names[k].chosen=0;
    }
    
 return 0;
}

/**
*  name_rooms
*
*  param: names 
*               ptr to name_option structs containing optional names and usage flag
*  param: rooms 
*               ptr to room structs 
*  return: Return_Description
*
*  details: Details
**/
int name_rooms(struct name_option* names, struct room* rooms){
    int r = 8;
    int m;
    // loop over all files
    for(m=0; m < 7; m++){
        int cont = 1;
        // continue until correct name is found
        while(cont == 1){
          
          // random number between 0-9
            r = rand()%10;
            // checking if name has been chosen already
           if( names[r].chosen == 0 ){
                printf("names[r: %d].chosen=%d\n",r,names[r].chosen); 
                // buffer for name (for clarity)
                char* buff = names[r].name;
                // checking buffer length
                if(strlen(buff) >0){
                    // copy the name into the room
                    strcpy(rooms[m].name, buff); 
                    // change conts to 0 (end loop)
                    cont=0;
                    // set chosen flag to 1 (has been chosen)
                    names[r].chosen=1;
                }
            }
         //printf("names[r: %d].chosen=%d\n",r,names[r].chosen); 
       }
    }
}

int initalize_connections(struct room* single_room, int num_connections){
    
    single_room->connections = (char*)malloc(num_connections*sizeof(char*));
    int n;
    
    for (n=0; n < num_connections; n++){
        single_room->connections[n]=(char*)malloc(56*sizeof(char));    
    }
    
}
int set_connections(struct name_option* names,struct room* rooms ){
    int m;
        for(m=0; m < 7; m++){
            int num_connections = rand()%7;
       //     printf("Inside set_connections: %s\n",rooms[m].name);
            initalize_connections(&rooms[m], num_connections);
/*             int cont = 1;
        while(cont == 1){
                r = rand()%7;
               if(names[r].chosen != 1){
                    strncpy(rooms[m].name, names[r].name, 56);
                     rooms[m].name[55]='\0';
                     printf("rooms[m]->name: %s\n",rooms[m].name);
                      names[r].chosen=1;
                 cont=0;
               } */
        }
    
    
}



// Thread-safe usage of strerror_r().
//http://www.ibm.com/developerworks/aix/library/au-errnovariable/
void thread_safe( int err )
{
    char buff[56];
    
    if( strerror_r( err, buff, 56 ) == 0 ) {
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


