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
	e->args = cmd->args;
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

int	is_builtin(t_msh *m, int forks)
{
	char	*builtins[] = {"echo", "cd", "pwd",
		"export", "unset", "env", "exit", NULL};
	if (s_in_s(m->ct->cmds[forks]->args[0], builtins))
		return (1);
	return (0);
}

int	exec_builtin(t_msh *m, int forks)
{
	char	*name;
	char	**args;

	ft_printf("Is a builtin!\n");
	args = m->ct->cmds[forks]->args;
	name = args[0];
	if (s_iseq(name, "echo"))
		ft_echo(m, args);
	else if (s_iseq(name, "cd"))
		ft_cd(m, args);
	else if (s_iseq(name, "pwd"))
		ft_pwd(m, args);
	else if (s_iseq(name, "export"))
		ft_export(m, args);
	else if (s_iseq(name, "unset"))
		ft_unset(m, args);
	else if (s_iseq(name, "env"))
		ft_env(m, args);
	else if (s_iseq(name, "exit"))
		ft_exit(m, args);
	return (0);
}

int	execute_cmd(t_msh *m, int forks)
{
	t_exec	*e;

	ft_printf("I exist: pid %i\n", getpid());
	if (is_builtin(m, forks))
	{
		exec_builtin(m, forks);
	}
	else
	{
		e = mk_exec(m->ct->cmds[forks]);
		if (e == NULL)
			return (1);
		if (execve(e->pathname, e->args, e->env) == -1)
			ft_printf("execve failed.\n");
		free(e);
	}
	return (0);
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
			if (infile && !s_iseq(infile, ""))
			{
				int fd_open = open(infile, O_RDONLY);
				//protect
				dup2(fd_open, STDIN_FILENO);
				close(fd_open);
			}
			else
			{
				if (forks != 0)
					dup2(fd[forks * 2 + 0], STDIN_FILENO);
			}

			close(fd[forks * 2 + 0]);
			char *outfile = m->ct->cmds[forks]->out;
			if (outfile && !s_iseq(outfile, ""))
			{
				int fd_open = open(outfile, O_WRONLY);
				//protect
				dup2(fd_open, STDOUT_FILENO);
				close(fd_open);
			}
			else
			{
				if (forks != m->ct->senc - 1)
					dup2(fd[(forks + 1) * 2 + 1], STDOUT_FILENO);
			}
			close_fds(forks * 2 + 0, fd, m->ct->senc + 1);
			if (execute_cmd(m, forks) == 1)
				return (1);
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
