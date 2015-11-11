#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> // used to reference size types
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include "room_tests.h"
#include "adventure.h"

//while (fgets(buffer, 100, fp)) {
//    printf("%s", buffer);
//}


int room_test1(){
    
    // test that 7 files exist
    
    
    
    
}
/* void test_pair_rooms(){
        room* first= make_room();
    strncpy(first->name, "first",strlen("first"));

   room* second = make_room();
    strncpy(second->name, "sec",strlen("sec"));
  
 second->num_connections++;
 second->connections[0]= first;  

    int result= pair_rooms(first,second);
    printf("first connection: %s\n", first->connections[0]->name);
    printf("result: %d\n",result);
    free_room(first);
   free_room(second);
} */