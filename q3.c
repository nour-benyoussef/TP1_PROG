#include <string.h>
#include <stdlib.h>
int main(void){
	int lenght;
	write(1,"Bienvenue dans le shell Ensea.\n",31);
	write(1,"Pour quitter, tapez exit.\n", 27);
	
	while(1){
	char buffer[10] = "";
	
	//Affichage d'un message d'acceuil
	write(1,"\n",1);
	write(1,"enseash % ", 10);
	
	lenght = read(0, buffer,10);
	buffer[lenght-1]=0;
	
	//Execution d'une commande sans quitter le shell
	pid_t pid=fork();
		if(pid==0){
		execlp(buffer,buffer, (char*)NULL);
		exit(EXIT_SUCCESS);
		}
		else{
		wait(NULL);
		}
		
	// gestion de la commande exit
		if(strcmp(buffer,"exit") ==0){
		break;
		}
	
			
	}
	write(1,"Bye bye... \n",13);
	return 0;
}
