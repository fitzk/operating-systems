

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

#define MAX_BUFF  1024
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
   
    char text_buffer[MAX_BUFF];
    char key_buffer[MAX_BUFF];
    char filename[30]; 
    char key[30];
    
    // usage
    if (argc < 4) {
        fprintf(stderr," usage: %s filename key port\n" , argv[0]);
        exit(0);
    }
    
    // get file name and key file name
    sprintf(filename,"%s",argv[1]);
    sprintf(key,"%s",argv[2]);
    // get port num
    portno = atoi(argv[3]);
    // create scoket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        error(" ERROR opening socket" );
    }
    // get host name
    server = gethostbyname("127.0.0.1");
    if (server == NULL) {
        fprintf(stderr," ERROR, no such hostn" );
        exit(0);
    }
    // server settings
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    
    // connect client and server
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
        error(" ERROR connecting" );
    }
    
    // file pointers to fp and key file
     FILE * fp;
     FILE* kfp;
     
    if( (fp= fopen(filename,"r")) == NULL ){
        perror(filename);
    } 
    if( (kfp = fopen(key,"r")) == NULL){
        perror(key);
    }


        // zero out buffers
    bzero(text_buffer,MAX_BUFF);
    bzero(key_buffer,MAX_BUFF);
    char encrypted_buffer[MAX_BUFF];
    bzero(encrypted_buffer,MAX_BUFF);

  //  printf("client!!\n");
 //  while( r = fread(text_buffer,1, MAX_BUFF-1,fp) , !feof(fp))
      do{
          fgets(text_buffer, MAX_BUFF-1,fp)  ;
//        printf("client) text_buffer: %s\n", text_buffer);
        // Reads key file into string
        fgets(key_buffer, MAX_BUFF,kfp);

        int bytes_sent = send(sockfd, text_buffer, MAX_BUFF, 0);
        
        if (bytes_sent < 0){ 
            error(" ERROR writing to socket" );
        }         
        
     //  printf("client!\n");
        bytes_sent = 0;
        bytes_sent = send(sockfd, key_buffer, MAX_BUFF, 0);
        if (bytes_sent < 0){ 
            error(" ERROR writing to socket" );
        }
      //  printf("Client) key bytes sent: %d\n", bytes_sent);
        


        bytes_sent = 0;
        bytes_sent = recv(sockfd,encrypted_buffer,MAX_BUFF, 0);
        if (bytes_sent < 0){ 
            error(" ERROR reading from socket" );
        }
        fprintf(stdout,"%s" ,encrypted_buffer); 
   }while(!feof(fp));
   fclose(fp);
   fclose(kfp);
    close(sockfd);
    return 0;
}