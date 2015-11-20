#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
int main(){
	while(1){
		int bg = 0;//0- not background 1- background
		printf("$#$>");//prompt
		char input[100];
		int length=1;
		char *cmd[10];
		memset(input, 0, sizeof(input));
		memset(cmd, 0, sizeof(cmd));
		fgets(input, 100, stdin);
		if(input[strlen(input)-2] == '&'){//if background
			bg = 1;	
			input[strlen(input)-2] == 0;
		}
		input[strlen(input)-1] = 0;

		cmd[0] = strtok(input,"|&");
		while(cmd[length] = strtok(NULL, "|&")){//decomposite the command
			length++;
		}	

		int id,pid,fd[length][2],status;

		for(id=0;id<length;id++){//2D array of pipes
			pipe(fd[id]);
		}
		for(id=length;id>0;id--){//fork a chain of process
			if((pid=fork()) != 0)
				break;
		}
		if(id < length){//child
			if(id!=(length-1)){
				if(dup2(fd[id][1],STDOUT_FILENO)==-1)
					perror("fail to redirect stdout");
			}
			if(id!=0){
				if(dup2(fd[id-1][0],STDIN_FILENO)==-1)
					perror("fail to redirect stdin");
			}
			int a;
			for(a=length;a>0;a--){
				close(fd[a-1][0]);
				close(fd[a-1][1]);
			}
			wait(&status);
			char *command = cmd[id];
			char *cmd_cp1 = strdup(cmd[id]);
			char *cmd_cp2 = strdup(cmd[id]);
			char *redirectout;
			char *redirectin;
			
			redirectout = strtok(cmd_cp2,">");
			redirectout = strtok(NULL," ><");
			redirectin = strtok(cmd_cp1,"<");
			redirectin = strtok(NULL," <>");
			
			if(redirectout){
				int outfd = open(redirectout,O_CREAT|O_WRONLY|O_TRUNC,00644);
				dup2(outfd,1);	
			}			
			if(redirectin){
				int infd = open(redirectin,O_RDONLY);
				dup2(infd,0);
			}
		
			command = strtok(command,"><");
			int arg_len = 1;
			char *arglist[4];
			arglist[0] = strtok(command," ");
			while(arglist[arg_len] = strtok(NULL, " ")){
				arg_len++;
			}
			arglist[arg_len] = 0;
			if(execvp(arglist[0],arglist)<0){
				perror("err in execvp");
			}
		}
		else{//parent
			int a;
			for(a=length;a>0;a--){
				close(fd[a-1][0]);
				close(fd[a-1][1]);
			}
			if(bg==0){
				waitpid(pid,&status,0);	
			}
		}
	}
}
