#include <time.h>
#include <stdlib.h>
#include <stdio.h>



int main(int argc, char *argv[])
{
    if(argc < 2){
        printf("usage: keygen <number of characters>\n");
        exit(1);
    }

    srand(time(NULL));
    
    int num_chars =atoi(argv[1]);
    int i;
    char legal_chars[]={'A','B', 'C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',' '};
    for(i=0; i < num_chars;i++){
        int r = (rand() %27);

        
        if(r > 27){
            r -=27;
        }
        printf("%c",legal_chars[r]);


    }
    
   
}