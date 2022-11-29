#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "libft/libft.h"
#include <sys/wait.h>

//number of pipes
#define N 3

void	close_fds(int pos, int *fd)
{
	while (pos < N * 2)
	{
		close(fd[pos]);
		pos++;
	}
}

int main(int argc, char **argv)
{
	int	*fd;
	int	i;
	int	pid;
	int	forks;

	fd = (int *)calloc(sizeof(int), N * 2);
	if (!fd)
		return (-1);
	i = 0;
	while (i < N)
	{
		if(pipe(fd + i * 2)){
			perror("pipe");
			return -1;
		}
		i++;
	}
	forks = 0;
	while (forks < N)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return -1;
		}
		else if (pid == 0)
		{
			// child
			close(fd[forks * 2 + 1]);
			dup2(fd[forks * 2 + 0], STDIN_FILENO);
			close(fd[forks * 2 + 0]);
			if (forks != N - 1)
				dup2(fd[(forks + 1) * 2 + 1], STDOUT_FILENO);
			close_fds(forks * 2 + 0, fd);
			execve("./cat", NULL, NULL);
		}
		else
		{
			// parent
			if (forks == 0)
			{
				char *tmp = get_next_line(0);
				write(fd[forks * 2 + 1], tmp, ft_strlen(tmp));
				free(tmp);
			}
			close(fd[forks * 2 + 0]);
			close(fd[forks * 2 + 1]);
			waitpid(pid, NULL, 0);
		}
		forks++;
	}
	free(fd);
	return 0;
}