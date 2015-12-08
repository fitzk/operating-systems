

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
   
   // get size of both files
    fseek(fp, -1, SEEK_END);
    size_t fp_size = ftell(fp) + 1;
    
    
    fseek(kfp, -1, SEEK_END);
    size_t kfp_size = ftell(kfp) + 1;
    
    
   // Moves buffer to start of the files
	fseek(fp, 0, SEEK_SET);
	fseek(kfp, 0, SEEK_SET);	
    
    // make sure key is at least as long as fp
    if(kfp_size < fp_size){
        printf("Error: %s is too short, %d vs. %d\n", key, kfp_size,fp_size);
        exit(1);
    }

        // zero out buffers
    bzero(text_buffer,MAX_BUFF);
    bzero(key_buffer,MAX_BUFF);
    char encrypted_buffer[MAX_BUFF];
            bzero(encrypted_buffer,MAX_BUFF);
    int r;
    int total;
   while( fgets(text_buffer, MAX_BUFF-1,fp) , !feof(fp)){  
        total = total + MAX_BUFF-1; 

//		printf("Client) fileno(fp): %d\n", fileno(fp));
        // printf("Client) r: %d\n", r);
     //  printf("Client) text_buffer: %s\n", text_buffer);
        // Reads key file into string
        fgets(key_buffer, MAX_BUFF-1,kfp);
        
        text_buffer[MAX_BUFF-1]='\0';
        key_buffer[MAX_BUFF-1]='\0'; 
        
        if(total >= fp_size){
            text_buffer[MAX_BUFF-1]='\0';
            key_buffer[MAX_BUFF-1]='\0'; 
        }
		/* 
		//key_buffer[r] = '\0';
        printf("Client) key_buffer: %s\n", key_buffer);
        */
        int bytes_sent = send(sockfd, text_buffer, MAX_BUFF, 0);
    //   printf("Client) text bytes sent: %d\n", bytes_sent);
        if (bytes_sent < 0){ 
            error(" ERROR writing to socket" );
        }         
        
        
        bytes_sent = 0;
        bytes_sent = send(sockfd, key_buffer, MAX_BUFF, 0);
        if (bytes_sent < 0){ 
            error(" ERROR writing to socket" );
        }
        // printf("Client) key bytes sent: %d\n", bytes_sent);
        


        bytes_sent = 0;
        bytes_sent = recv(sockfd,encrypted_buffer,MAX_BUFF, 0);
        if (bytes_sent < 0){ 
            error(" ERROR reading from socket" );
        }
        fprintf(stdout,"%s" ,encrypted_buffer); 
   }
    close(sockfd);
    return 0;
}