/* smallsh.c */
#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE -1
#define PATH_MAX 500
    // getcwd(char*buf,size_t size) 
    // function returns a null-terminated string containing an absolute pathname that is
    // the current working directory of the calling process
int cwd_test(){
    char* cwd;
    char buff[PATH_MAX + 1];
    cwd = getcwd( buff, PATH_MAX + 1 );
    if( cwd != NULL ) {
        printf( "My working directory is %s.\n", cwd );
    }
    return EXIT_SUCCESS;
    
}
    // chdir(const char*path) changes the current working directory of the calling process
    // to the directory specified in path
int chdir_test(){

    const char* path = "test_dir";
    if( chdir( path) == 0 ) {
        printf( "Directory changed to %s\n", path);
        return EXIT_SUCCESS;
    } else {
        perror(path);
        return EXIT_FAILURE;
    }
}

int open_test(){
	char* pathname = "tofile";
	int flags;
	int mode;
    int fd = open(pathname,flags,mode);
    if (fd == -1){        
        perror(pathname);
    }
    
    if(close(fd)== -1){
        perror(pathname);
    }
}

void fork_test(){
    pid_t spawnpid = -5;
    int ten = 10;
    
    spawnpid = fork();
    switch(spawnpid){
        
        case -1: 
            perror("fork failed");
            exit(1);
            break;
        case 0:
            ten = ten + 1;
            printf("I am the child! ten = %d\n",ten);
            break;
        default:
            ten = ten -1;
            printf("I am the parent! ten = %d\n", ten);
            break;
    }
    printf("This will be executed by both of us!\n");
}

void cd_smallsh(char* path){
 

    


    

    // chroot() changes the root directory of the calling process to that specified in path. This
    // directory will be used for pathnames beginning with /. The root directory is inhereted by 
    // all children of the calling process
    
    
}

main(){
    
    int result = cwd_test();
    printf("result: %d\n", result);
     int result_chdir = chdir_test();
     

}



