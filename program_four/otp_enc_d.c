/*rt number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#define MAX_BUFF 1024

void error(const char *msg)
{
    perror(msg);
    exit(1);
}
char encode(char text_char, char key_char){
    
    int text = (int) text_char;
    int key = (int) key_char;

    if(text == 32){
        text = 26;
    }else{
        text = text - 65;
    }
    if(key == 32){
        key = 26;
    }else{
        key = key - 65;
    }
  //  printf("text: %d key: %d \n", text,key);
    
    int encoded = (text + key) % 27;
   // printf("text + key: %d\n",text + key);
   // printf("encoded: %d\n",encoded);
    
    if(encoded == 26){
            encoded = 32;
    }else{
        encoded = encoded + 65;
    }
    
    char encoded_char = (char) encoded;
   // printf("encoded: %c\n", encoded_char);
    return encoded_char;
    
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    
    char text_buffer[MAX_BUFF]; // buffer for plain text file
    char key_buffer[MAX_BUFF]; // buffer for key
    int status;
    struct sockaddr_in serv_addr, cli_addr;
    int n, text_n, key_n;
    if (argc < 2) {
        fprintf(stderr," ERROR, no port providedn" );
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error(" ERROR opening socket" );
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        error(" ERROR on binding" );
    }
    
    listen(sockfd,5);
    while(1){
        
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            error(" Error: on accept" );
        }
            int pid = fork();

            if (pid == -1) {
                perror("fork");
                exit(1);
            }else if (pid == 0) {
            //    printf("child!");
        
                // read from text_buffer

            bzero(text_buffer,MAX_BUFF);
           bzero(key_buffer,MAX_BUFF);

           while(( text_n = recv(newsockfd,text_buffer,MAX_BUFF,0)) >0){                   
                         //   printf("size text received: %d\n", key_n);
                        if (text_n < 0) {
                            error(" ERROR reading from socket" );
                        }                   
            //             printf("Server) Here is the message: %s\n" ,text_buffer);

                        key_n = recv(newsockfd,key_buffer,MAX_BUFF,0);
                        if (key_n < 0) {
                            error(" ERROR reading from socket" );
                        }   
                        char ret_buffer[MAX_BUFF];
                        int idx;
                        for(idx = 0; idx < strlen(text_buffer); idx++){
                            ret_buffer[idx] = encode(text_buffer[idx],key_buffer[idx]);
                        } 
                        int bytes_sent = send(newsockfd, ret_buffer,MAX_BUFF,0);

                        if (bytes_sent < 0){
                            error(" ERROR writing to socket" );
                        }
                        
           }
         break;
        }else{
             do {
                    pid = waitpid(-1, &status, WNOHANG);
                } while (pid > 0);
                
        }

    }
         
    exit(0);
}