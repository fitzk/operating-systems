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
int to_integer(char alpha){
    
    int ascii = (int) alpha;
    
    if(ascii == 32){
        return 27;
    }
    return ascii - 65;
    
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    
    char text_buffer[MAX_BUFF]; // buffer for plain text file
    char key_buffer[MAX_BUFF]; // buffer for key
    
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
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) {
        error(" ERROR on accept" );
    }
    
    // read from text_buffer
bzero(text_buffer,MAX_BUFF);

text_n = recv(newsockfd,text_buffer,MAX_BUFF-1,0);
printf("size key received: %d\n", key_n);
if (text_n < 0) {
    error(" ERROR reading from socket" );
}
    


     // read from key_buffer
/*     bzero(key_buffer,MAX_BUFF);
do{    
    key_n = recv(newsockfd,text_buffer,MAX_BUFF-1,0);
    if (key_n < 0) {
        error(" ERROR reading from socket" );
    }
    printf("size key received: %d\n", key_n);
     }while(key_n > 0); */
    
    
    char ret_buffer[MAX_BUFF];
/*     int idx;
    for(idx = 0; idx < strlen(text_buffer); idx++){
        printf("key char: %c\n", key_buffer[idx]);
    int a =  to_integer(text_buffer[idx]);
    int b = to_integer(key_buffer[idx]);
    int temp =  (a +b)%27;
        printf("text: %d\n", a);
        printf("key: %d\n", b);
    } */
     n = send(newsockfd, text_buffer,MAX_BUFF-1,0);
   // printf(" Here is the message: %s\n" ,ret_buffer);
    
    if (n < 0){
        error(" ERROR writing to socket" );
    }
    close(newsockfd);
    close(sockfd);
    return 0;
}