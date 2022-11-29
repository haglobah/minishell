#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "libft/libft.h"

int main(int argc, char **argv)
{
    int fd1[2];
    int fd2[2];
    char buf[] = "HELLO WORLD!";
    if(pipe(fd1)){
      perror("pipe");
      return -1;
    }
	if(pipe(fd2)){
      perror("pipe");
      return -1;
    }
    switch(fork()){
        case -1:
            perror("fork");
            return -1;
        case 0:
            // child
            close(fd1[1]);
            dup2(fd1[0], STDIN_FILENO);
            close(fd1[0]);
			dup2(fd2[1], STDOUT_FILENO);
			close(fd2[1]);
			close(fd2[0]);
            execve("./cat", NULL, NULL);
        default:
            // parent
            close(fd1[0]);
			char *tmp = get_next_line(0);
            write(fd1[1], tmp, ft_strlen(tmp));
			free(tmp);
            close(fd1[1]);
            wait(NULL);
    }
	switch(fork()){
        case -1:
            perror("fork");
            return -1;
        case 0:
            // child
            close(fd2[1]);
            dup2(fd2[0], STDIN_FILENO);
            close(fd2[0]);
            execve("./cat", NULL, NULL);
        default:
            // parent
            close(fd2[0]);
            close(fd2[1]);
            wait(NULL);
    }
    printf("END~\n");
    return 0;
}