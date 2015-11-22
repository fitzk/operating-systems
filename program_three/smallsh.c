/* smallsh.c */
#include<sys/types.h>
#include<sys/wait.h>

#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<signal.h>
#include<sys/fcntl.h>

#define	MAX
#define PATH_MAX 500
#define ARGS_MAX 512
#define STR_MAX 50
typedef struct Args Args;
typedef struct Process_Env Process_Env;

struct Args{

	char input_str[2048];
	char**input_arr;
	int length;
	char command_path[2048];
	char* execv_args[ARGS_MAX];
	int execv_args_length;
	int redirect_stdout;
	int redirect_stdin;
	int background_process;
	char* prev_status;

};

struct Process_Env{
	int pid;
	char path_str[2048];
	char**path_arr;
	int length;
};



void status_smallsh(int stat){
 
    printf("exit value %d\n", stat);
    
}



/*
 * str_split
 *
 * parameters:
 * 	str - string to be split
 * 	array - char array for tokens to be stored in
 * 	len	- length variable to update token count in external variable
 * 	character - the character for the string to be split on
 * comments:
 * 	I wrote my own strtok functon because I was getting a couple of
 * 	memory leaks using the built in strtok.
 *
 * */
void str_split(char str[],char**array,int*len, const char character){
	int length = strlen(str);
	char* token =(char *) malloc(sizeof(char)*STR_MAX);
	// printf("length: %d, full path: %s\n",length,str);
	// clear token of garbage
	int y;
	for(y=0; y < STR_MAX; y++){
		token[y]='\0';
	}
	int tk_count=0;
	int arr_count = 0;
	int x;
	for(x=0; x < length; x++){
		if(str[x]!= character && str[x] != '\n'){
			// add character to token

			token[tk_count]=str[x];
			tk_count++;
		}else if(str[x] == character || str[x] == '\n'){
			if(str[x]=='\n')
			{
				char*end=&token[tk_count];
				sprintf(end,"%c",'\0');
			}

			// store token to input array
			sprintf(array[arr_count],"%s\0",token);

			// clean up token
			for(y=0; y < STR_MAX; y++){
				token[y]='\0';
			}
			// reset token size
			tk_count=0;
			//update number of commands
			arr_count++;

		}

	}

	*len=arr_count;
	free(token);
}

void set_redirect_background_flags(Args*args){

	int x;
	for(x=0; x < args->length; x++){

		if(compstrings(">", args->input_arr[x])== 0){
			args->redirect_stdout = 1;
		}
		if(compstrings("<", args->input_arr[x])== 0){
			args->redirect_stdin = 1;
		}
		if(compstrings("&", args->input_arr[x])== 0){
            printf("check if setting background_process in set_redirect_background_flags!\n");
			args->background_process = 1;
		}
	}
}

void get_path(Process_Env* env){
	sprintf(env->path_str, "%s",getenv("PATH"));
	int length = strlen(env->path_str);
	env->path_str[length]='\n';
}
/*
 * get_output_file
 *
 * searches args for output file
 *
 * */
void get_output_file(Args*args,char*file){

	int x;
	for(x=0; x < args->length; x++){

		int redir_output = compstrings(">", args->input_arr[x]);
		if(redir_output == 0){
			sprintf(file,"%s",args->input_arr[x+1]);
			break;
		}
	}
}
/*
 * get_input_file
 *
 * searches args for output file
 *
 * */
void get_input_file(Args*args,char*file){

	int x;
	for(x=0; x < args->length; x++){

		int redir_input = compstrings("<", args->input_arr[x]);
		if(redir_input == 0){

			sprintf(file,"%s",args->input_arr[x+1]);
			break;
		}
	}
}
/*
 * formats args to call execv with
 *
 * */
void set_execv_args(Args*args){
	int x=0;
	args->execv_args[x]= args->command_path;
	args->execv_args_length++;
	if(args->length > 1){
		for(x=1; x < args->length; x++){

			if(compstrings("<",args->input_arr[x])== 0){
				break;
			}
			if(compstrings(">",args->input_arr[x])== 0){
				break;
			}
			if(compstrings("&",args->input_arr[x])== 0){
				break;
			}
			if(args->input_arr[x]!= NULL){
				args->execv_args[x]=args->input_arr[x];
				args->execv_args_length++;
			}
		}
	}
	args->execv_args[args->execv_args_length]=NULL;
	args->execv_args_length++;
}
/*
 * helper function to print out execv args
 *
 * */
void print_execv_args(Args*args){
	printf("pre execv command_path: %s\n", args->command_path);
	int r;
	for(r=0; r < args->execv_args_length; r++){
		printf("execv_args: %s\n", args->execv_args[r]);
	}
}
/*
 * run_execv
 *
 * checks if child process is calling
 * and if so calls execv
 * */
void run_execv(Args*args, int process){

	if(process == 0){
		if(execv(args->command_path,args->execv_args)== -1){
			perror("execv");
		}
	}
}
void termination_signal(int signo) {
	printf("terminated by signal %d\n", signo);
	kill(getpid(), SIGINT);
}
/*
 * spawns a new process and returns the pid
 *
 * */
pid_t create_new_process(Args *args){
	pid_t spawnpid = -5;
	struct sigaction sig;
	sig.sa_flags = 0;
	sig.sa_handler = termination_signal;
	sigfillset(&(sig.sa_mask));

    sigaction(SIGINT, &sig, NULL);
	spawnpid = fork();
	switch(spawnpid){

		case -1:{
			perror("fork failed");
			exit(1);
			break;
		}
		case 0:{
			return spawnpid;
			break;
		}
		default:{
            if(args->background_process == 0){
                int returnStatus;
                int options = 0;

                waitpid(-1,&returnStatus,options);
                if(returnStatus == 1){
                    perror("child process");
                    exit(1);
                }
            }else{
                    printf("background pid is %d\n",spawnpid);
                }
			return spawnpid;
			break;
		}
	}
}
/*
 * redirects standard output for foreground and background processes
 *
 * */
void redirect_stdout(Args*args){
	int fd_stdout = dup(1);


		char*output_file = malloc(sizeof(char)*STR_MAX);
		get_output_file(args,output_file);
		// open file for writing output
		int fd;
		fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if(fd == -1){
			perror(output_file);
			exit(1);
		}
		free(output_file);
	    int fd2 = dup2(fd,1);
		if(fd2 == -1){
			perror("dup2");
			exit(1);
		}
}
/*
 * redirects standard input for foreground and background processes
 *
 * */

int redirect_stdin(Args*args){

	fflush(STDIN_FILENO);

    int fd;
	char*input_file = malloc(sizeof(char)*STR_MAX);
	get_input_file(args,input_file);
    if(args->background_process == 1){
        printf("args->background_process %d for %d", args->background_process, getpid());
         fd = open("/dev/null", O_RDONLY);
    }else{
         fd = open(input_file,O_RDONLY);
        if(fd == -1) {
            printf("smallsh: cannot open %s for input\n", input_file);
            exit(1);
        }
    }
	int fd2 = dup2(fd, 0);
	if(fd2 == -1){
		perror("dup2");
		exit(1);
	}
	free(input_file);
	return fd2;
}
/*
 * resets stdin
 * */
void reset_stdin(int fd2){
	dup2(fd2,0);
	close(fd2);

}

/*
 * calls input and output redirection function calls
 * */
void external_command(Args*args, Process_Env * env){

    if(args->background_process == 1){
        args->redirect_stdin =1;
    }
	if(args->redirect_stdin == 1 && args->redirect_stdout == 1){
		pid_t process = create_new_process(args);
		if(process == 0){
			redirect_stdin(args);
			redirect_stdout(args);
			run_execv(args,process);
		}
	}else if(args->redirect_stdin == 1 && args->redirect_stdout == 0){
		pid_t process = create_new_process(args);
		if(process == 0){
			int fd = redirect_stdin(args);
			run_execv(args,process);
			reset_stdin(fd);
		}
	}else if(args->redirect_stdout == 1 && args->redirect_stdin == 0){

		pid_t process = create_new_process(args);
		if(process == 0){
			redirect_stdout(args);
			run_execv(args,process);
		}
	}else if(args->redirect_stdin == 0 && args->redirect_stdout== 0){
		pid_t process = create_new_process(args);
		run_execv(args,process);
	}

}



/*
 * sets the correct path to the command issued by user
 *
 * */
void get_command_path(Args*args, Process_Env* env){
	char* path =(char*) malloc(sizeof(char)*(STR_MAX*2));

		int x;
		for(x=0; x < env->length; x++){
			sprintf(path, "%s/%s\0",env->path_arr[x],args->input_arr[0]);

			if(access(path,F_OK)== 0)
				break;
		}
		sprintf(args->command_path, "%s",path);
	free(path);
}
/*
 * controls command execution
 *
 * */
void command_handler(Args*args, Process_Env* env){

	char expr;
	if(compstrings("cd",args->input_arr[0])== 0){
		expr = 'c';
	}else if(compstrings("stats",args->input_arr[0])== 0){
		expr = 's';
	}else if(compstrings("exit",args->input_arr[0])== 0){
		expr = 'e';
	}else if(compstrings("#",args->input_arr[0])== 0){
		expr = '#';
	}

	switch(expr){
		case 'c':{
			printf("cd!\n");
			break;
		}
		case 's':{
			printf("stats!\n");
			break;
		}
		case 'e':{
			printf("exit!\n");
			break;
		}case '#':{
			break;
		}default:{
			//printf("external!\n");

			external_command(args, env);

			break;
		}

	}
}

/*
 *
 * */
void blunt_object(){
	pid_t pid;
	int status;
	pid = waitpid(-1, &status, WNOHANG);
	if(pid > 0){

		if (WIFEXITED(status)) {
			printf("background pid %d has finished, exit value %d\n", pid, WEXITSTATUS(status));
		}

		if (WIFSIGNALED(status)) {
			printf("background pid %d has finished, terminated by signal %d\n", pid, WTERMSIG(status));
		}
	}else if(pid == -1){
        perror("zombies");
    }
}

/*
 * compares two strings
 * returns 0 if match, 1 if no match
 * */
int compstrings( const char*s1, const char*s2)
{

    if(strlen(s1)== 0 || strlen(s2)== 0){
    	return 1;
    }
    int result = strncmp(s1,s2,strlen(s1));
    if(result != 0){
    	return 1;
    }
    return 0;
}
/*
 * main
 *
 * */
main(int argc, char*argv[]){

	int exit_args=-1;
	printf("smallsh\n");
	const char *quit = "exit\0";

	Process_Env* env =(Process_Env*) malloc(sizeof(Process_Env));
	env->pid = getpid();
	env->path_arr =(char**)malloc(sizeof(char*)*PATH_MAX);
	get_path(env);

	int x;
	for(x=0; x < PATH_MAX; x++){
		env->path_arr[x]=(char*)malloc(sizeof(char)*STR_MAX);
	}
	str_split(env->path_str,env->path_arr,&env->length,':');


	do{
		fflush(STDIN_FILENO);

		Args* args = (Args*)malloc(sizeof(Args));
		args->input_arr =(char**) malloc(sizeof(char*)*ARGS_MAX);

		int y;
		for(y=0; y < ARGS_MAX; y++){
			args->input_arr[y]=(char*)malloc(sizeof(char)*STR_MAX);
		//	args->execv_args[y]=(char*)malloc(sizeof(char*));
		}
		args->redirect_stdin=0;
		args->redirect_stdout=0;
		args->background_process=0;

		printf(":");
		fgets(args->input_str,2048,stdin);
		int length = strlen(args->input_str);
		//printf("length: %d\n", length);
		if(length > 1 ){
			str_split(args->input_str,args->input_arr,&args->length, ' ');


			// check if redirection or background process in command
			// set flags in struct
			set_redirect_background_flags(args);
			// get's the correct path to the current command
			// and stores it to the args struct
			get_command_path(args,env);
			// set options array
			args->execv_args_length=0;
			set_execv_args(args);


			command_handler(args,env);

			exit_args = compstrings(quit,args->input_arr[0]);
     
		}
		int m;
		for(m=0; m < ARGS_MAX; m++){
			free(args->input_arr[m]);
			//free(args->execv_args[m]);
		}
   //    printf("loop\n");
		free(args->input_arr);
		//free(args->execv_args);
		free(args);
        blunt_object();
	}while(exit_args != 0);


	for(x=0; x < PATH_MAX; x++){
		free(env->path_arr[x]);
	}


	free(env->path_arr);
	free(env);
	return 0;
}



