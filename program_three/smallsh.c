/*
 * Kayla Fitzsimmons
 * (fitzsimk)
 * 11/23/2015
 * CS344
 * Program 4
 * smallsh.c
 *
 * */
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

// struct to hold user's commands
// and args
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

};
// struct to hold environment information
// for current process
struct Process_Env{
	int pid;
	char path_str[2048];
	char**path_arr;
	int length;
};

/*
 * global to hold the previous
 * foreground command status
 *
 * */
char prev_status[2048];

/*
 * status internal command
 * */
void status_smallsh(){
 
	printf("%s", prev_status);
    
}
/*
 * cd internal command
 *
 * */
void cd_smallsh(Args *args){
	if (args->length == 1) {
		chdir(getenv("HOME"));
	}
	else {
		if (chdir(args->input_arr[1]) == -1) {
			printf("smallsh: cd: %s: No such file or directory\n", args->input_arr[1]);
		}
	}
}

/*
 * str_split
 *
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
/*
 * sets flags for user input, output redirection, and
 * background processes
 * */
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
         	args->background_process = 1;
		}
	}
}
/*
 * set's the current env path in env struct
 *
 * */
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

		int result = execv(args->command_path,args->execv_args);
		if(result == -1){
			perror(args->input_arr[0]);
			exit(1);
		}

}
void catch_fg(int signo) {
	int returnStatus;

	if(WIFSIGNALED(&returnStatus) > 0 ){
			printf("CATCH_FG: terminated by signal %d\n", WTERMSIG(returnStatus));
			sprintf(prev_status,"CATCH_FG: terminated by signal %d\n", WTERMSIG(returnStatus));
			//	kill(getpid(), SIGINT);
			//	exit(1);
		}

}
/*
 * foreground process
 * forks a new process and returns the pid
 *
 * */
pid_t foreground_process(Args *args){
	pid_t spawnpid = -5;


	spawnpid = fork();
	switch(spawnpid){

		case -1:{
			perror("fork failed");
			exit(1);
			break;
		}
		case 0:{
			struct sigaction catch;
			catch.sa_flags = 0;
			catch.sa_handler = catch_fg;
			sigfillset(&(catch.sa_mask));
			sigaction(SIGINT, &catch, NULL);
			return spawnpid;
			break;
		}
		default:{
			int returnStatus;
			do{

			int options = 0;

			pid_t pid = waitpid(spawnpid,&returnStatus,0);
			if(returnStatus == 1){
				perror("child process");
				exit(1);
			}

				if(WIFEXITED(returnStatus) > 0) {
					sprintf(prev_status,"foreground process %d has finished, exit value %d\n", pid, WEXITSTATUS(returnStatus));

				}
				if(WIFSIGNALED(returnStatus) > 0){
					printf("terminated by signal %d\n", pid, WTERMSIG(returnStatus));
					sprintf(prev_status,"terminated by signal %d\n", pid, WTERMSIG(returnStatus));
				}

			}while(!WIFEXITED(returnStatus)&& !WIFSIGNALED(returnStatus));
			return spawnpid;
			break;
		}
	}
}
/*
 * background process
 * forks a new process and returns the pid
 *
 * */
pid_t background_process(Args *args){
	pid_t spawnpid = -5;
	struct sigaction signal;
	signal.sa_handler = SIG_IGN;
	signal.sa_flags = 0;

	sigfillset(&(signal.sa_mask));
	sigaction(SIGINT, &signal, NULL);

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

            printf("background pid is %d\n",spawnpid);
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
 * redirects background stdin to keep terminal clear
 *
 * */
int redirect_background_stdin(Args*args){
	fflush(STDIN_FILENO);
    int fd1 = open("/dev/null", O_RDONLY);
    int std_in = dup2(fd1, 0);
    return std_in;
}
/*
 * redirects background stdout to keep terminal clear
 *
 * */
int redirect_background_stdout(Args*args){
    int fd1 = open("/dev/null", O_WRONLY);
    int std_out = dup2(fd1, 1);
    return std_out;
}
/*
 * redirects standard input for foreground processes
 *
 * */

int redirect_stdin(Args*args){

	fflush(STDIN_FILENO);
	int std_in;
    int fd;
	char*input_file = malloc(sizeof(char)*STR_MAX);
	get_input_file(args,input_file);

        fd = open(input_file,O_RDONLY);
        if(fd == -1) {
            printf("smallsh: cannot open %s for input\n", input_file);
            exit(1);
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
 * calls input and output redirection function calls
 * for all possible input/output redirection combos
 *
 * */
void foreground_command(Args*args, Process_Env * env){


	if(args->redirect_stdin == 1 && args->redirect_stdout == 1){
		pid_t process = foreground_process(args);
		if(process == 0){
			redirect_stdin(args);
			redirect_stdout(args);
			run_execv(args,process);
		}
	}else if(args->redirect_stdin == 1 && args->redirect_stdout == 0){
		pid_t process = foreground_process(args);
		if(process == 0){
			int fd = redirect_stdin(args);
			run_execv(args,process);
		}
	}else if(args->redirect_stdout == 1 && args->redirect_stdin == 0){

		pid_t process = foreground_process(args);
		if(process == 0){
			redirect_stdout(args);
			run_execv(args,process);
		}
	}else if(args->redirect_stdin == 0 && args->redirect_stdout== 0){
		pid_t process = foreground_process(args);
		if(process == 0){
			run_execv(args,process);
		}
	}

}
/*
 * calls input and output redirection function calls
 * */
void background_command(Args*args, Process_Env * env){


	pid_t process =  background_process(args);

	if(process == 0){
		redirect_background_stdin(args);
		redirect_background_stdout(args);
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
	}else if(compstrings("status",args->input_arr[0])== 0){
		expr = 's';
	}else if(compstrings("exit",args->input_arr[0])== 0){
		expr = 'e';
	}else if(compstrings("#",args->input_arr[0])== 0){
		expr = '#';
	}

	switch(expr){
		case 'c':{
			cd_smallsh(args);
			break;
		}
		case 's':{
			status_smallsh();
			break;
		}
		case 'e':{
			break;
		}case '#':{
			break;
		}default:{
			//printf("external!\n");

			if(args->background_process == 0){
				foreground_command(args, env);
			}else if(args->background_process == 1){
				background_command(args,env);
			}

			break;
		}

	}
}

/*
 * kills any zombie children
 * */
void blunt_object(){
	pid_t pid;
	int status;
	pid = waitpid(-1, &status, WNOHANG);
	if(pid > 0){

		if (WIFEXITED(status)) {
			printf("background pid %d has finished, exit value %d\n", pid, WEXITSTATUS(status));
			//exit(0);
		}

		if (WIFSIGNALED(status)) {
			printf("background pid %d has finished, terminated by signal %d\n", pid, WTERMSIG(status));
			//exit(1);
		}
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
int main(int argc, char*argv[]){

	int exit_args=-1;
	const char *quit = "exit\0";

	// set path information struct
	Process_Env* env =(Process_Env*) malloc(sizeof(Process_Env));
	env->pid = getpid();
	env->path_arr =(char**)malloc(sizeof(char*)*PATH_MAX);
	get_path(env);

	int x;
	for(x=0; x < PATH_MAX; x++){
		env->path_arr[x]=(char*)malloc(sizeof(char)*STR_MAX);
	}
	str_split(env->path_str,env->path_arr,&env->length,':');

	// ignore ctrl-c in main process
	struct sigaction ignore;
	ignore.sa_handler = SIG_IGN;
	ignore.sa_flags = 0;
	sigfillset(&(ignore.sa_mask));



	do{
		sigaction(SIGINT, &ignore, NULL);
		blunt_object();
		fflush(STDIN_FILENO);


		Args* args = (Args*)malloc(sizeof(Args));
		args->input_arr =(char**) malloc(sizeof(char*)*ARGS_MAX);

		int y;
		for(y=0; y < ARGS_MAX; y++){
			args->input_arr[y]=(char*)malloc(sizeof(char)*STR_MAX);
		}
		args->redirect_stdin=0;
		args->redirect_stdout=0;
		args->background_process=0;

		printf(":");
		fgets(args->input_str,2048,stdin);
		int length = strlen(args->input_str);

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

			// directs commands
			command_handler(args,env);

			// exit if command is exit
			exit_args = compstrings(quit,args->input_arr[0]);
		}

		// clean up
		int m;
		for(m=0; m < ARGS_MAX; m++){
			free(args->input_arr[m]);
		}

		free(args->input_arr);
		free(args);
        blunt_object();
	}while(exit_args != 0);

	// clean up
	blunt_object();
	for(x=0; x < PATH_MAX; x++){
		free(env->path_arr[x]);
	}
	free(env->path_arr);
	free(env);
	return 0;
}



