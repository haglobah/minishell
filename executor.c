/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhagenlo <bhagenlo@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 09:39:54 by bhagenlo          #+#    #+#             */
/*   Updated: 2022/12/03 14:16:57 by bhagenlo         ###   ########.fr       */
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

t_execve	*mk_execve(t_cmd *cmd)
{
	t_execve	*ev;

	ev = ft_calloc(sizeof(t_execve), 1);
	if (ev == NULL)
		return (NULL);
	ev->args = cmd->args;
	ev->pathname = find_path(ev->args);
	ev->env = clone_env();
	return (ev);
}

void	del_execve(t_execve *ev)
{
	free_strs(ev->args);
	free(ev->pathname);
	free_strs(ev->env);
	free(ev);
}

void	close_fds(int pos, int *fd, int num_pipes)
{
	while (pos < num_pipes * 2)
	{
		close(fd[pos]);
		pos++;
	}
}

int	is_builtin(t_cmd *cmd)
{
	char	*builtins[] = {"echo", "cd", "pwd",
		"export", "unset", "env", "exit", NULL};
	if (s_in_s(cmd->args[0], builtins))
		return (1);
	return (0);
}

int	exec_builtin_from_child(t_msh *m, int forks)
{
	char	*name;
	char	**args;

	ft_printf("Called from child!\n");
	args = m->ct->cmds[forks]->args;
	name = args[0];
	if (s_iseq(name, "echo"))
	{
		ft_echo(m, args);
		exit(0);
	}
	else 
	{
		ft_printf("%s did not execute. It was called from child.\n", name);
		exit(0);
	}
	return (0);
}

int	exec_builtin(t_msh *m, int forks)
{
	char	*name;
	char	**args;

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
	t_execve	*ev;
	t_cmd		*cmd;

	ft_printf("I exist: pid %i\n", getpid());
	cmd = m->ct->cmds[forks];
	if (is_builtin(cmd))
	{
		exec_builtin_from_child(m, forks);
	}
	else
	{
		ev = mk_execve(m->ct->cmds[forks]);
		if (ev == NULL)
			return (1);
		if (execve(ev->pathname, ev->args, ev->env) == -1)
			ft_printf("execve failed.\n");
		del_execve(ev);
	}
	return (0);
}

int	*mk_pipes(t_msh *m)
{
	int	*fd;
	int i;

	fd = (int *)calloc(sizeof(int), (m->ct->senc + 1) * 2);
	if (!fd)
		return (NULL);
	i = -1;
	while (++i < m->ct->senc + 1)
	{
		if(pipe(fd + i * 2)){
			perror("pipe");
			return (NULL);
		}
	}
	return (fd);
}

int	setup_in(t_msh *m, int *fd, int forks)
{
	char	*infile;
	int		fd_open;

	close(fd[forks * 2 + 1]);
	infile = m->ct->cmds[forks]->in;
	if (infile && !s_iseq(infile, ""))
	{
		fd_open = open(infile, O_RDONLY);
		if (fd_open == -1)
			return (1);
		dup2(fd_open, STDIN_FILENO);
		close(fd_open);
	}
	else
	{
		if (forks != 0)
			dup2(fd[forks * 2 + 0], STDIN_FILENO);
	}
	return (0);
}

int	setup_out(t_msh *m, int *fd, int forks)
{
	char	*outfile;
	int		fd_open;

	close(fd[forks * 2 + 0]);
	outfile = m->ct->cmds[forks]->out;
	if (outfile && !s_iseq(outfile, ""))
	{
		fd_open = open(outfile, O_WRONLY);
		if (fd_open == -1)
			return (1);
		dup2(fd_open, STDOUT_FILENO);
		close(fd_open);
	}
	else
	{
		if (forks != m->ct->senc - 1)
			dup2(fd[(forks + 1) * 2 + 1], STDOUT_FILENO);
	}
	return (0);
}

int	run_child(t_msh *m, int *fd, int forks)
{
	setup_in(m, fd, forks);
	setup_out(m, fd, forks);
	close_fds(forks * 2 + 0, fd, m->ct->senc + 1);
	if (execute_cmd(m, forks) == 1)
		return (1);
	return (0);
}

int	run_parent(t_msh *m, int *fd, int forks)
{
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
	return (0);
}

int	execute_only_cmds(t_msh *m)
{
	int	*fd;
	int	pid;
	int	forks;

	fd = mk_pipes(m);
	if (fd == NULL)
		return (0);
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
			//child
			if(run_child(m, fd, forks) == 1)
				return (1);
		}
		else
		{
			// parent
			run_parent(m, fd, forks);
			waitpid(pid, NULL, 0);
		}
		forks++;
	}
	free(fd);
	return (0);
}

int	run_builtin(t_msh *m, int *fd, int forks)
{
	ft_printf("Executed builtin as parent.\n");
	exec_builtin(m, forks);
	return (0);
}

int	exec_cmds_builtin(t_msh *m)
{
	int	*fd;
	int	pid;
	int	forks;

	fd = mk_pipes(m);
	if (fd == NULL)
		return (0);
	forks = 0;
	while (forks < m->ct->senc - 1)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return -1;
		}
		else if (pid == 0)
		{
			//child
			if(run_child(m, fd, forks) == 1)
				return (1);
		}
		else
		{
			// parent
			run_parent(m, fd, forks);
			waitpid(pid, NULL, 0);
		}
		forks++;
	}
	run_builtin(m, fd, forks);
	free(fd);
	return (0);
}

int	is_last_builtin(t_msh *m)
{
	int	i;
	t_cmd	*last;

	i += -1;
	while (m->ct->cmds[++i])
		;
	last = m->ct->cmds[i - 1];
	return (is_builtin(last));
}

int	execute(t_msh *m)
{
	if (is_last_builtin(m))
	{
		exec_cmds_builtin(m);
	}
	else
	{
		execute_only_cmds(m);
	}
	return (1);
}
