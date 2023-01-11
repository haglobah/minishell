/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 21:03:51 by tpeters           #+#    #+#             */
/*   Updated: 2023/01/11 21:04:18 by tpeters          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

int	execute_only_cmds_sub(t_msh *m, int *fd, int forks)
{
	int	pid;
	int	status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	else if (pid == 0)
	{
		if (run_child(m, fd, forks) == 1)
			return (1);
	}
	else
	{
		run_parent(m, fd, forks);
		if (forks == m->ct->senc - 1)
		{
			waitpid(pid, &status, 0);
			*(m->rv) = WEXITSTATUS(status);
		}
	}
	return (0);
}

int	execute_only_cmds(t_msh *m)
{
	int	*fd;
	int	forks;
	int	tmp_return;

	fd = mk_pipes(m);
	if (fd == NULL)
		return (0);
	forks = 0;
	while (forks < m->ct->senc)
	{
		tmp_return = execute_only_cmds_sub(m, fd, forks);
		if (tmp_return)
			return (tmp_return);
		forks++;
	}
	while (1)
	{
		if (wait(NULL) <= 0)
			break ;
	}
	ft_free((void **)&fd);
	return (0);
}

int	run_builtin(t_msh *m, int *fd, int forks)
{
	return (exec_builtin(m, forks, fd));
}

int	exec_cmds_builtin_sub(t_msh *m, int *forks, int *fd)
{
	int	pid;

	*forks = 0;
	while (*forks < m->ct->senc - 1)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return (-1);
		}
		else if (pid == 0)
		{
			if (run_child(m, fd, *forks) == 1)
			{
				ft_free((void **)&fd);
				return (1);
			}
		}
		else
			run_parent(m, fd, *forks);
		(*forks)++;
	}
	return (0);
}

void	exec_cmds_builtin_sub2(t_msh *m, int *fd, int forks)
{
	int	fd_open;
	int	tmpfd;

	if (!(m->ct->cmds[forks]->appp))
		unlink(m->ct->cmds[forks]->out);
	fd_open = open(m->ct->cmds[forks]->out, O_WRONLY | O_CREAT | O_APPEND \
	* (m->ct->cmds[forks]->appp), 0666);
	tmpfd = dup(STDOUT_FILENO);
	dup2(fd_open, STDOUT_FILENO);
	close(fd_open);
	*(m->rv) = run_builtin(m, fd, forks);
	dup2(tmpfd, STDOUT_FILENO);
	close(tmpfd);
}
