/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhagenlo <bhagenlo@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 09:39:54 by bhagenlo          #+#    #+#             */
/*   Updated: 2022/12/03 19:40:14 by bhagenlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

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
		return (ft_echo(m, args));
	else if (s_iseq(name, "cd"))
		return (ft_cd(m, args));
	else if (s_iseq(name, "pwd"))
		return (ft_pwd(m, args));
	else if (s_iseq(name, "export"))
	{
		return (ft_export(m, args));
		// printns(m->env);
	}
	else if (s_iseq(name, "unset"))
		return (ft_unset(m, args));
	else if (s_iseq(name, "env"))
		return (ft_env(m, args));
	else if (s_iseq(name, "exit"))
		return (ft_exit(m, args));
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
		ev = mk_execve(m, m->ct->cmds[forks]);
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
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
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
			if (forks == m->ct->senc - 1)
			{
				int	status;
				waitpid(pid, &status, 0);
				*(m->rv) = WEXITSTATUS(status);
			}
		}
		forks++;
	}
	while (1)
	{
		if (wait(NULL) <= 0)
			break ;
	}
	free(fd);
	return (0);
}

int	run_builtin(t_msh *m, int *fd, int forks)
{
	(void)fd;
	ft_printf("Executed builtin as parent.\n");
	return (exec_builtin(m, forks));
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
		}
		forks++;
	}
	*(m->rv) = run_builtin(m, fd, forks);
	while (1)
	{
		if (wait(NULL) <= 0)
			break ;
	}
	free(fd);
	return (0);
}

int	is_last_builtin(t_msh *m)
{
	int	i;
	t_cmd	*last;

	i = -1;
	while (m->ct->cmds[++i])
		;
	last = m->ct->cmds[i - 1];
	return (is_builtin(last));
}

int	execute(t_msh *m)
{
	g_our_global = 1;
	if (is_last_builtin(m))
	{
		exec_cmds_builtin(m);
	}
	else
	{
		execute_only_cmds(m);
	}
	g_our_global = 0;
	return (1);
}
