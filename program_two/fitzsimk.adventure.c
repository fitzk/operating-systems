/**
* author: Kayla Fitzsimmons
* course: Operating Systems
* term: Fall 2015
* program: 2
* file: fitzsimk.adventure.c
* description: File I/O practice game. 
*
*                    First, program generates content for files. There are 10 name_option
*                    structs with the 10 name options stored in them and a flag for if they
*                    are chosen or not for the game. 
*
*                    Once all the names are chosen an array of room structs is created
*                    for each name. The connections are set, and the types are set.
*
*                    Files are created and the file content in the room structs is stored in
*                    them.
*                     
*                    The room structs are then freed and can no longer be accessed. Then
*                    a new set of room structs (new_rooms) is created and the file content
*                    is read into them.
*
*                   To hold the player information I created a player struct. 
*
*                   The game is ran from the main function.
*
*comments: I chose this structure in order to keep all the information organized as it
*                   was created, read, and stored.
* 
**/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include<string.h>

#define NUM_ROOMS 7
#define MAX_USER_PATH 200
#define MAX_PATH_LENGTH 100
#define MAX_CONNECTIONS 6
#define MAX_NAME_LENGTH 10

typedef enum TYPE TYPE;
typedef struct name_option name_option;
typedef struct room room;
typedef struct player player;

// room types
enum TYPE {
    START_ROOM=0,
    MID_ROOM,
    END_ROOM   
};
/**
*  name_option
*  details: 'name' is one of the 10 name options
*              'chosen' is a flag that is set if the name
*               was randomly chosen to be used in the 
*               game
**/
struct name_option {
    
    char* name;
     int chosen;
    
} ;
/**
*  room
*  details: 'name' is the name of the room
*               'connections' are pointers to adjacent rooms
*               'type' is the room type
*               'num_connections' is the number of adjacent rooms
*
**/
struct room{
    
    char* name;
    room** connections;
    TYPE type;
    int num_connections;
    
};
/**
*  player
*  details: 'route' is the path the player has taken through the rooms
*               'current' is the current room
*               'route_length' is the number of steps the user has taken 
*
**/
struct player{
    room** route;
    room* current;
    int route_length;
};

/**
*  make_name_option
*  return: pointer to name_option struct
*  calling function: 
*  details: allocates memory for name_option struct
**/
name_option* make_name_option() {
    name_option* no = malloc(sizeof(name_option));
    no->name = malloc(MAX_NAME_LENGTH * sizeof(char));
    no->chosen = 0;
    return no;
}
/**
*  free_names
*  param: names
*  return: n/a
*  calling function: 
*  details: frees memory for name option structs
**/
void free_names(name_option**names){

    int m;
    for(m=0; m < 10; m++){
        free(names[m]->name);
        free(names[m]);
    }
    free(names);
    
}
/**
*  make_name_option_array
*  return: double pointer to name_option structs
*  calling function: gen_file_content
*  details: calls make_name_option()
**/
name_option** make_name_option_array()
   {
           // create10 name_option structs to hold naming schema
        name_option** names= malloc(sizeof(name_option*)*10);
           int h;
        for(h=0; h < 10;  h++){

            names[h] = make_name_option();
         
        } 
        return names;
 }
 /**
*  define_file_names 
*  param: names 
*  return: 0 on success
*  details: sets name member in all name_option structs, and sets chosen flag to 0
**/
int define_file_names(name_option**names){

    // array of potential file names
    const char* ch_names[]={"aah\0","bah\0","cah\0","dah\0","ehh\0","fah\0","gah\0","hha\0","iah\0","jah\0"};
    
    // assigning names to structs 
    int k;
    for(k=0; k < 10;k++){
       strncpy(names[k]->name, ch_names[k],4);
    }
    
 return 0;
}
/**
*  free_user
*  param: user player struct
*  details: frees up memory allocated for player struct
**/
void free_user(player* user){
    int idx;
    free(user->route);
    free(user->current);
    free(user);
}

/**
*  make_room
*  return: pointer to room struct
*  details: allocates memory for room struct
**/
room* make_room(){
    room* rm = malloc(sizeof(room));
    rm->name = malloc(MAX_NAME_LENGTH * sizeof(char));
    rm->connections=malloc(MAX_CONNECTIONS*sizeof(room*));
    rm->type= MID_ROOM;
    rm->num_connections = 0;
    return rm;
}
/**
*  make_room_array
*  return: double pointer to room struct array
*  details: calls make_room()
**/
room** make_room_array(){
    
    room** rooms= malloc(sizeof(room*)*NUM_ROOMS);
    int idx;
    
    for(idx=0; idx < NUM_ROOMS; idx++){

       rooms[idx]=make_room();
    }
    return rooms;
    
}
/**
*  free_room
*  param: single_rm pointer to room struct
*  details: frees memory allocated for room struct
**/
void free_room(room* single_rm){
        free(single_rm->name);
        free(single_rm->connections);
        free(single_rm);
}
/**
*  free_rooms
*  param: rooms- double pointer access 7 room structs
*  return: n/a
*  details: frees data for rooms struct array
**/
void free_rooms(room**rooms){
    
    
    int m;
    for(m=0; m < NUM_ROOMS; m++){
        free_room(rooms[m]);
    }
   free(rooms);
    
}

/**
*  name_rooms
*  param: names - ptr to name_option structs containing optional names and usage flag
*  param: rooms - double pointer access 7 room structs 
*  return: 0 on success
*  details: Details
**/
int name_rooms(name_option**names, room**rooms){
    int r = 8;
    int m;
    // loop over all files
    for(m=0; m < NUM_ROOMS; m++){
        int cont = 1;
        // continue until correct name is found
        while(cont == 1){
          
          // random number between 0-9
            r = rand()%10;
 #if (PRINT_NM_RMS == 1)
        printf("random number r:  %d\n\n", r);
 #endif
            // checking if name has been chosen already
           if( names[r]->chosen == 0 ){

                // buffer for name (for clarity)
             char* buff = names[r]->name;
#if (PRINT_NM_RMS == 1)
              printf("length: %d\n",strlen(buff));
              printf("buffer: %s\n",buff);
#endif
                // checking buffer length
                if(strlen(buff) >0){
                    // copy the name into the room
                    strncpy(rooms[m]->name, buff,5); 
                 // change conts to 0 (end loop)
#if (PRINT_NM_RMS == 1)
                printf("rooms[m: %d]->name= %s\n",m,rooms[m]->name); 
#endif
                    cont=0;
                    // set chosen flag to 1 (has been chosen)
                    names[r]->chosen=1;
                 
                    
                }
            }
       }
    }
}
/**
*  set_type
*  param: rooms - double pointer access 7 room structs
*  return: 0 on success 
*  details: sets start and end type for two randomly
*              chosen structs
**/
int set_type(room** rooms){
    int set_start=0, set_end=0;
    do{
            if(set_start == 0){
                
                set_start = rand()%NUM_ROOMS;
                rooms[set_start]->type = START_ROOM;
            }
            if(set_end == 0){
                set_end = rand()%NUM_ROOMS;
                if(set_start != set_end){
                    rooms[set_end]->type = END_ROOM;
                }else{
                    set_end = 0;
                }
            }
        
        }while(set_start ==0 && set_end ==0);
    return 0;
}
/**
*  set_connections
*  param: names - ptr to array of name_option structs
*  param: rooms- double pointer access 7 room structs 
*  return: 0 on success
*  details: sets 3-6 connections in each room struct
**/
int set_connections(room ** rooms){
  
   int m;
   for(m=0; m < NUM_ROOMS; m++){      
        int num_connections = 2;
        while(num_connections > 0  || rooms[m]->num_connections < 3){
            if(rooms[m]->num_connections == 6)
                break;
            int   r = rand()% NUM_ROOMS;
            if(strncmp(rooms[m]->name, rooms[r]->name,5) != 0){
                int result = pair_rooms(rooms[m], rooms[r]);
                if(result == 0){
                    num_connections--; 
                }
            } 
        }
    } 

return 0; 
}

/**
*  pair_rooms
*  param: first  pointer to room struct
*  param: second pointer to room struct
*  return: 1 if already connected, 2 if first is 
*             already a connection for second,
*             and 0 for new connection
*  details: Pairs up rooms as connections to eachother
*              performs necessary checks for existing connections
*              and partial connections.
**/
int pair_rooms(room* first, room*second){
    int m;
    int c1;
    int c2;

    int n;
     // check if already connected
    if( first->num_connections > 0 ){
        for(n=0; n < first->num_connections; n++){
            if (strncmp(second->name, first->connections[n]->name,4) == 0){
              return  1;
           }
        }
    }
    //check if set as connection for second room
    if( second->num_connections > 0 ){
        for(m=0; m < second->num_connections; m++){

       if (strncmp(first->name, second->connections[m]->name,4) == 0){
                 first->num_connections++;
                 c1= first->num_connections-1;
                 first->connections[c1]=second;
                  return 2;
           }
        }
    }
       
   // if the first two conditions are not met, a new pair is formed
    first->num_connections++;
    c1= first->num_connections-1;
    first->connections[c1]=second;
        
    
    second->num_connections++;
    c2= second->num_connections-1;
    second->connections[c2]=first;
    
    return 0;
}
/**
*  print_rooms
*  param: rooms- double pointer access 7 room structs
*  details: helper function to print room content
**/
void print_rooms(room** rooms){
    int total_connections=0;
    int m;
    for(m=0; m < NUM_ROOMS; m++){
        printf("Room %d\n",m);
        printf("\tname: %s\n",rooms[m]->name);
        total_connections+=rooms[m]->num_connections;
       printf("\t# connections: %d\n",rooms[m]->num_connections);
       printf("\t\t connections: \n");
        int n;
        for(n=0; n<rooms[m]->num_connections; n++){
            printf("\t\t%s\n",rooms[m]->connections[n]->name);
        }    
       printf("\t# type: %d\n",rooms[m]->type);
         printf("\n\n");
    }
}
/**
*  gen_file_content
*  param: rooms - double pointer access 7 room structs
*  return: 0 on success
*  details: driver function to call functions that
*              generate initial room content 
**/
int gen_file_content(room** rooms){
   
    name_option** names= make_name_option_array();
    define_file_names(names);
    name_rooms(names, rooms);   
    set_type(rooms);
    set_connections(rooms);
    free_names(names);

   return 0;
}

/**
*  get_full_path
*  param: dir_path- Path to fitzsimk.rooms.<pid> folder 
*  param: file_name Parameter_Description
*  param: full_path Parameter_Description
*  return: 0 on completion
*  details: concats dir_path and the file name to form 
*              full_path
**/
int get_full_path(char *dir_path, char * file_name, char* full_path){

    sprintf(full_path, "%s/%s", dir_path, file_name);   
    
    return 0;
}
/**
*  get_cs_type
*  param: type    Parameter_Description
*  param: cs_type Parameter_Description 
*  details: assigns chara string version of type
*              to cs_type param based on type
**/
void get_cs_type(TYPE type, char* cs_type){
    
    if(type == 0){
        sprintf(cs_type, "%s","START_ROOM");
    }else if(type == 1){
        sprintf(cs_type, "%s", "MID_ROOM");
    }else if(type == 2){
        sprintf(cs_type, "%s","END_ROOM");
    }
    
}
/**
*  create_files
*  param: dir_path- Path to fitzsimk.rooms.<pid> folder
*  param: 
*  details: creates each file and inserts
*              formatted room content
**/
void create_files(char* dir_path,room**rooms ){
    int m;
    char* full_path;
    char* cs_type;
    for(m=0; m < NUM_ROOMS; m++){
        full_path = (char *) malloc(sizeof(char) * MAX_PATH_LENGTH); 
        get_full_path(dir_path, rooms[m]->name, full_path);
        FILE*  fp = fopen(full_path, "w");
        free(full_path);
        fprintf(fp,"ROOM NAME: %s\n", rooms[m]->name);
        int n;
        for(n=0; n < rooms[m]->num_connections; n++){

            fprintf(fp,"CONNECTION %d: %s\n",n+1,rooms[m]->connections[n]->name);      
        }
        cs_type = (char *) malloc(sizeof(char) * 20); 
        get_cs_type(rooms[m]->type, cs_type);
        fprintf(fp, "ROOM TYPE: %s\n", cs_type);
        free(cs_type);
        fclose(fp);
    }

}

/**
*  create_directory
*  return: dir_path
*  details: Creates a directory in the format fitzsimk.rooms.<current pid>
*               initial file permissions set to 755
**/
char* create_directory(){
     
    int pid=getpid();
   char* dir_path = malloc(sizeof(char)* 60);
    sprintf(dir_path,"fitzsimk.rooms.%d",pid);

   if( mkdir(dir_path,0755) != 0) {
       printf("File was not created...\n");
       exit(1);
   }   
    return dir_path;
} 
/**
*  get_type_enum
*  param: cs_type Parameter_Description
*  return: type of room
*  details: compares type string read in file and returns
*              cooresponding type
**/
TYPE get_type_enum(char* cs_type){
    if(strncmp(cs_type,"START_ROOM",10)==0){
        return START_ROOM;
    }
    if(strncmp(cs_type,"MID_ROOM",8)==0){
        return MID_ROOM;
    }
   if(strncmp(cs_type,"END_ROOM",8)==0){
        return END_ROOM;
    }
}
 /**
 *  read_file
 *  param: dir_path- Path to fitzsimk.rooms.<pid> folder 
 *  param: new_rooms - double pointer access 7 room structs   
 *  param: single_room - pointer to room struct
 *  details: Because the room name is already read in
 *              and assigned to the new_rooms structs by the
 *              read_names function, the read_file function only
 *              reads in and stores the connections and room type,
 *              for the single_room struct. get_room is called in 
 *              this scope to add room pointers based on connections
 *              read from file.
 **/
 void read_file(char* dir_path, room**new_rooms, room * single_room){

    char* full_path = (char *) malloc(sizeof(char) * MAX_PATH_LENGTH); 
    char* first_word =(char *) malloc(sizeof(char) * 30);  
    char* second_word =(char *) malloc(sizeof(char) * 30); 
    char* third_word =(char *) malloc(sizeof(char) * 30); 
    char* connection = "CONNECTION";
    char* type= "TYPE:";
    //path to dir 
    get_full_path(dir_path, single_room->name, full_path);
    FILE* fp = fopen(full_path, "rb");
    int c;
    do{
        ungetc(c,fp);
        fscanf(fp, "%s %s %s\n", first_word, second_word, third_word);
        //check for connection
        if(strncmp(connection, first_word, sizeof(first_word))==0){
            // assigns the matching room struct as a connection
            // for the current room
           get_room(third_word, new_rooms,single_room); 
           
        }
        //check for room type
       if(strncmp(type, second_word, sizeof(second_word))==0){
                   single_room->type = get_type_enum (third_word);
        }

        c = getc(fp);
        }while(c != EOF);

    free(full_path);
    free(third_word);
    free(second_word);
    free(first_word);
    fclose(fp);  
}
/**
*  get_room
*  param: name - room struct name
*  param: new_rooms - double pointer access 7 room structs   
*  param: single_room - pointer to room struct
*  return: 0 for found, 1 for not found
*  details: This function assigns pointers to room structs
*               as connections for single_room, based on matching
*               the room name passed to this function, and the room
*               name in the associated struct in new_rooms
**/
int get_room(char* name,room**new_rooms, room* single_room){
    int m;
    for(m=0; m < NUM_ROOMS; m++){
         if(strncmp(name, new_rooms[m]->name, sizeof(name))==0){
                   single_room->connections[single_room->num_connections]= new_rooms[m];
                   single_room->num_connections++;       
                   return 0;
         }
    }
    return 1;
}
/**
*  read_all_files
*  param: dir_path
*  param: new_rooms - double pointer access 7 room structs
*  details: Driver function for reading file contents,
*               calls read_file for each file in dir
**/
void read_all_files(char* dir_path, room**new_rooms){
    int idx;
    for(idx = 0; idx < 7; idx++){
        
     read_file(dir_path, new_rooms, new_rooms[idx]);
        
    } 
}

/**
*  read_names
*  param: dir_path- Path to fitzsimk.rooms.<pid> folder
*  param: new_rooms - double pointer access 7 room structs
*  details: This function reads the file names from the 
*               generated directory and stores them in the 
*               new room structs. No information about the 
*               rooms persists after file contents are generated.
*               All information is read from files. 
**/
void read_names(char*dir_path, room** new_rooms){
    int count=0;
    DIR* dir;
   if((dir = opendir(dir_path)) == NULL){
       printf("file was not opened");
   }
    struct dirent* dp;
    char * file_name;
        
        while ((dp=readdir(dir)) != NULL) {
            file_name = dp->d_name;
            if(strncmp(file_name, ".",2) !=0 &&strncmp(file_name, "..",2) !=0  ){
                          sprintf(new_rooms[count]->name, "%s",file_name);
                        count++;
            }
     }
     if(count != NUM_ROOMS){
         perror("in function: read_names ... error not all files assigned");
     }
        closedir(dir);
}
/**
*  get_start_index
*  param: new_rooms - double pointer access 7 room structs
*  return: index of room with type 'START_ROOM'
*  details: iterates over each room in struct
**/
int get_start_index(room** new_rooms){
    
    int idx; 
    for(idx=0; idx < NUM_ROOMS; idx++){
        if(new_rooms[idx]->type == START_ROOM){
            return idx;
        }
    }
}

/**
* main
* return: 0
* details: runs program 
**/
int main(void){
    // set random seed
    srand(time(NULL));
    // create file contents
    room** rooms = make_room_array();
    gen_file_content(rooms);  
    // create the directory and output 
    // room contents to files
    char* dir_path=create_directory();
    create_files(dir_path, rooms);
    // **old rooms are freed and can no longer be accessed**
    free_rooms(rooms);
    // create new set of room structs to read 
    // information from files
    room** new_rooms = make_room_array();
    // read in file names
    read_names(dir_path,new_rooms);
    // read in files
    read_all_files(dir_path, new_rooms);
    
    // allocate memory for user
    player*user = malloc(sizeof(player));
    user->route = malloc(MAX_USER_PATH * sizeof(room*));
    user->route_length = 0;
    user->current=malloc(sizeof(room*));
   
    //setting start index
    int start_idx= get_start_index(new_rooms);
    user->current = new_rooms[start_idx];
    
    // while the user's current room isn't the end room
    while(user->current->type != 2){
        // input char array
        char* input = malloc(sizeof(char)*100);
        
        // print out current location
        printf("CURRENT LOCATION: %s\n", user->current->name);
        // print out connecting rooms
        printf("POSSIBLE CONNECTIONS:");

        int idx;
        for(idx=0; idx < user->current->num_connections; idx++){
            // if it's the last connection change formatting to add
            // period and newline
            if(idx == (user->current->num_connections - 1)){
                printf(" %s.\n", user->current->connections[idx]->name);
            }else{
                printf(" %s,", user->current->connections[idx]->name);
            }
        }
        // incorrect input flag
        int flag=0;
        // get input
        printf("WHERE TO? >");
        fgets(input,100,stdin);
        // check connections
        int x;
        for(x=0; x < user->current->num_connections; x++){
           // if there is a match
            if( strncmp(input,user->current->connections[x]->name, 3) == 0 ){
                // update route info
               user->route[user->route_length] = user->current;
               user->route_length++;
               // set current to the requested room
               user->current = user->current->connections[x];
               // just making sure my flag is set correctly
               flag=0;
               break;
            }else{
                // set error flag to true
                flag = 1;
            }
        }
        // output error message
        if(flag == 1){
                printf("\nHUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n"); 
        }
            printf("\n");
             free(input);
     };
    
    // add final room to route and update route length
    user->route[user->route_length] = user->current;
    user->route_length++; 
    
    // print congrats message and route info
    // *** does not print start room in route or include start room in
    //       route length as per the assingment example ***
    printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\nYOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", user->route_length-1);
    int route_idx;
    for(route_idx=1; route_idx < user->route_length; route_idx++){      
         printf("%s\n", user->route[route_idx]->name);
     }
     
    // clean up
    free_user(user);
    free_rooms(new_rooms);
    free(dir_path);
    
    return 0;
}