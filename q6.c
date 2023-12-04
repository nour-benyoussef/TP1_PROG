#include <unistd.h>  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h> 
#include <time.h>

int main(void) {
    int length, length_ex, length_sig;
    int ret_ex;
    int ret_sig;
    int accum;
    
    char str1[50];
    char str2[50];
    write(1, "Bienvenue dans le shell Ensea.\n", 31);
    write(1, "Pour quitter, tapez exit.\n", 27);
    
    while (1) {
        char buffer[100] = "";
        int status, exit_status, signal_status;
        struct timespec start, stop;
        
       
        write(1, "\n", 1);
        write(1, "enseash", 7);
        
        if (ret_sig == 1) {
            length_sig = sprintf(str1, "[signal:%d|%d ms]", signal_status, accum);
            write(1, str1, length_sig);
        }
        
        if (ret_ex == 1) {
            length_ex = sprintf(str2, "[exit:%d | %d ms] ", exit_status, accum);
            write(1, str2, length_ex);
        }
        
        write(1, " % ", 3);
        
        length = read(0, buffer, 100);
        buffer[length - 1] = 0;
        ret_ex = 0;
        ret_sig = 0;
        accum = 0;
        
        if (strcmp(buffer, "exit") == 0) {
            break;
        }
        
        clock_gettime(CLOCK_REALTIME, &start);
        
       
        pid_t pid = fork();
        if (pid == 0) {
            
            char *args[20]; 
            char *token = strtok(buffer, " ");
            int tokenIndex = 0;
            
            while (token != NULL) {
                args[tokenIndex++] = token;
                token = strtok(NULL, " ");
            }
            args[tokenIndex] = NULL;    
            execvp(args[0], args);
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
    
    write(1, "Bye bye... \n", 13);
    return 0;
}

