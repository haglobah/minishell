/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 09:39:54 by bhagenlo          #+#    #+#             */
/*   Updated: 2022/12/01 17:43:06 by tpeters          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

char **cons_args(t_cmd *cmd)
{
	int	i;
	int	j;
	int	empty_count;
	char	**args;

	empty_count = 0;
	i = -1;
	while (++i < cmd->argc)
	{
		if (s_iseq(cmd->argv[i], ""))
			empty_count += 1;
	}
	args = ft_calloc(sizeof(char *), (i - empty_count + 1));
	if (args == NULL)
		return (NULL);
	i = -1;
	j = 0;
	while (++i < cmd->argc)
	{
		if (s_iseq(cmd->argv[i], ""))
			continue ;
		args[j] = ft_calloc(sizeof(char), ft_strlen(cmd->argv[i]) + 1);
		if (args[j] == NULL)
			return (NULL);
		ft_strcpy(args[j], cmd->argv[i]);
		j++;
	}
	return (args);
}

char	*find_path(char **args)
{
	char	*path;

	path = ft_calloc(sizeof(char), ft_strlen(args[0]) + 1);
	if (path == NULL)
		return (NULL);
	//find the path (missing)
	ft_strcpy(path, args[0]);
	ft_printf("path: %s\n", path);
	return (path);
}

char	**cons_env()
{
	extern char	**environ;

	//change the env

	return (environ);
}

t_exec	*mk_exec(t_cmd *cmd)
{
	t_exec	*e;

	e = ft_calloc(sizeof(t_exec), 1);
	if (e == NULL)
		return (NULL);
	e->args = cons_args(cmd);
	// ft_printf("here\n");
	e->pathname = find_path(e->args);
	// ft_printf("%s\n", e->pathname);
	e->env = cons_env();
	return (e);
}

void	del_exec(t_exec *e)
{
	free_strs(e->args);
	free(e->pathname);
	free_strs(e->env);
	free(e);
}

void	close_fds(int pos, int *fd, int num_pipes)
{
	while (pos < num_pipes * 2)
	{
		close(fd[pos]);
		pos++;
	}
}

int	execute_all_cmds(t_msh *m)
{
	int	*fd;
	int	i;
	int	pid;
	int	forks;

	fd = (int *)calloc(sizeof(int), (m->ct->senc + 1) * 2);
	if (!fd)
		return (-1);
	i = 0;
	while (i < m->ct->senc + 1)
	{
		if(pipe(fd + i * 2)){
			perror("pipe");
			return -1;
		}
		i++;
	}
	forks = 0;
	while (forks < m->ct->senc)
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
			
			char *infile = m->ct->cmds[forks]->in;
			if (infile)
			{
				int fd_open = open(infile, O_RDONLY);
				dup2(fd_open, STDIN_FILENO);
				close(fd_open);
			}
			else
			{
				if (forks != 0)
					dup2(fd[forks * 2 + 0], STDIN_FILENO);
			}
			close(fd[forks * 2 + 0]);
			if (forks != m->ct->senc - 1)
				dup2(fd[(forks + 1) * 2 + 1], STDOUT_FILENO);
			close_fds(forks * 2 + 0, fd, m->ct->senc + 1);
			t_exec	*e;
			ft_printf("I exist: pid %i\n", getpid());
 			e = mk_exec(m->ct->cmds[forks]);
 			if (e == NULL)
 				return (1);
 			if (execve(e->pathname, e->args, e->env) == -1)
 				ft_printf("execve failed.\n");
 			free(e);
		}
		else
		{
			// parent
			if (m->ct->cmds[0]->in && forks == 0)
			{
				int fdin;
				fdin = open(m->ct->cmds[0]->in, O_RDONLY);
				char *tmp = get_next_line(fdin);
				while (tmp)
				{
					free(tmp);
					write(fd[forks * 2 + 1], tmp, ft_strlen(tmp));
					tmp = get_next_line(fdin);
				}
				if (fdin)
					close(fdin);
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

int	execute(t_msh *m)
{
	execute_all_cmds(m);
	return (1);
}
