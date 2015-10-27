#ifndef _ADVENTURE_H
#define _ADVENTURE_H
/**
* author: Kayla Fitzsimmons
* course: Operating Systems
* term: Fall 2015
* program: 2
* file: adventure.h
* description: header file for adventure.c
* ...
**/

#define START_ROOM =0
#define MID_ROOM =1
#define END_ROOM =2

struct name_option{
    char name[50];
    int chosen;
};

struct room{
    
    char name[50];
    const char* connections[6];
    const int type;    
    
};

int create_directory();
int define_file_names(struct name_option **);
#endif