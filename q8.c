#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>

#define MAX_SIZE 100

int main(void) {
    char buffer[MAX_SIZE] = {0};
   char str1[50];
    char str2[50];
    int buffer_size;
    int pid;
    int status;
    int exit_status = 0;
    int signal_status = 0;
    int ret_ex = 0;
    int ret_sig = 0;
    int accum = 0;
    int pipe_fd[2];
    int length, length_ex, length_sig;
      struct timespec start, stop;

   write(1, "Bienvenue dans le shell Ensea.\n", 31);
    write(1, "Pour quitter, tapez exit.\n", 27);
    

    while (1) {
       write(1, "\n", 1);
        write(1, "enseash", 7);
        
        
         if (ret_sig == 1) {
                 length_sig = sprintf(str1, "[signal:%d|%d ms]", signal_status, accum);
                write(1, str1,  length_sig );
            }
        
            if (ret_ex == 1) {
               length_ex = sprintf(str2, "[exit:%d|%d ms] ", exit_status, accum);
                write(1, str2, length_ex);
            }
         write(1, " % ", 3);
        buffer_size = read(STDIN_FILENO, buffer, MAX_SIZE);
        buffer[buffer_size - 1] = '\0';

        if (strcmp(buffer, "exit") == 0) {
            break;
        }

        clock_gettime(CLOCK_REALTIME, &start);


        exit_status = 0;
        signal_status = 0;
        ret_ex = 0;
        ret_sig = 0;
        accum = 0;

        pid = fork();
        if (pid == 0) {
            char *left;
            char *right;
            char *argv[MAX_SIZE / 2] = {NULL};
            int i = 0;
            int fd;

            left = strtok(buffer, ">");
            right = strtok(NULL, ">");
            if (right != NULL) {
                fd = open(right, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
                if (fd == -1) {
                    perror("Erreur lors de l'ouverture du fichier en sortie");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
                strcpy(buffer, left);
            }

            left = strtok(buffer, "<");
            right = strtok(NULL, "<");
            if (right != NULL) {
            right = strtok(right, " \t\n"); 
                fd = open(right, O_RDONLY);
                
                if (fd == -1) {
                    perror("Erreur lors de l'ouverture du fichier en entrée");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
                strcpy(buffer, left);
            }
            
            
            
        //******************************pipe*********************************************//    
 
     

		char *left_pipe = strtok(buffer, "|");
		char *right_pipe = strtok(NULL, "|");

		if (right_pipe != NULL) {
		    int pipefd[2];
		    if (pipe(pipefd) == -1) {
			perror("Erreur lors de la création du pipe");
			exit(EXIT_FAILURE);
		    }

		    pid_t pid_pipe = fork();
		    if (pid_pipe == 0) {
			close(pipefd[0]); 
			dup2(pipefd[1], STDOUT_FILENO); 
			close(pipefd[1]); 

			
			char *argv_pipe[MAX_SIZE / 2] = {NULL};
			int i_pipe = 0;
			argv_pipe[0] = strtok(left_pipe, " ");
			while (argv_pipe[i_pipe] != NULL) {
			    i_pipe++;
			    argv_pipe[i_pipe] = strtok(NULL, " ");
			}
			execvp(argv_pipe[0], argv_pipe);
			exit(EXIT_FAILURE);
		    } else {
			close(pipefd[1]); 
			waitpid(pid_pipe, NULL, 0);
			
			
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]); 

		       
			char *argv_pipe[MAX_SIZE / 2] = {NULL};
			int i_pipe = 0;
			argv_pipe[0] = strtok(right_pipe, " ");
			while (argv_pipe[i_pipe] != NULL) {
			    i_pipe++;
			    argv_pipe[i_pipe] = strtok(NULL, " ");
			}
			execvp(argv_pipe[0], argv_pipe);
			exit(EXIT_FAILURE);
		    }
		}


            
           //*****************************************************************************************************// 
            
            
            
            
            

            argv[0] = strtok(buffer, " ");
            while (argv[i] != NULL) {
                i++;
                argv[i] = strtok(NULL, " ");
            }

            execvp(argv[0], argv);
            exit(EXIT_FAILURE);
        } else {
            wait(&status);
            clock_gettime(CLOCK_REALTIME, &stop);
            accum = (stop.tv_nsec - start.tv_nsec) / 1000000;

           
            if (WIFEXITED(status)) {
                exit_status = WEXITSTATUS(status);
                ret_ex = 1;
            } else if (WIFSIGNALED(status)) {
                signal_status = WTERMSIG(status);
                ret_sig = 1;
            }

          
        }
    }

    write(STDOUT_FILENO, "Bye bye... \n", 13);
    return 0;
}

