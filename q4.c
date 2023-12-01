#include <unistd.h>  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h> 

int main(void){
	int lenght, length_ex, length_sig;
	int ret_ex;
	int ret_sig;
	char str1 [20];
	char str2 [20];
	write(1,"Bienvenue dans le shell Ensea.\n",31);
	write(1,"Pour quitter, tapez exit.\n", 27);
	
	while(1){
	char buffer[10] = "";
	int status,exit_status,signal_status;
	
	
	//Affichage d'un message d'acceuil
	write(1,"\n",1);
	write(1,"enseash", 7);
	
	if(ret_sig == 1){
	length_sig=sprintf(str1,"[signal:%d]",signal_status);
	write(1,str1,length_sig);
	}
	
	if(ret_ex == 1){
	length_ex =sprintf(str2,"[exit:%d] ",exit_status);
	write(1,str2,length_ex);
	}
	
	
	
	write(1," % ",3);
	
	lenght = read(0, buffer,10);
	buffer[lenght-1]=0;
	ret_ex=0;
	ret_sig=0;
	
	if(strcmp(buffer,"exit") ==0){
		break;
	}
	//Execution d'une commande sans quitter le shell
	pid_t pid=fork();
		if(pid==0){
		execlp(buffer,buffer, (char*)NULL);
		exit(EXIT_FAILURE);
		}
		else{
		wait(&status);
			if(WIFEXITED(status)){
				exit_status = WEXITSTATUS(status);
				ret_ex =1;
			}
			 else if (WIFSIGNALED(status)){
				signal_status = WTERMSIG(status);

				ret_sig=1;
			}
		}
		
	// gestion de la commande exit
		
	
			
	}
	write(1,"Bye bye... \n",13);
	return 0;
}
