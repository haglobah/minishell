/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 09:39:54 by bhagenlo          #+#    #+#             */
/*   Updated: 2023/01/11 20:40:46 by tpeters          ###   ########.fr       */
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
	char	**builtins;

	if (cmd->args == NULL)
		return (0);
	builtins = mk_strlist(7, "echo", "cd", "pwd",
			"export", "unset", "env", "exit");
	if (!builtins)
		return (0);
	if (s_in_s(cmd->args[0], builtins))
	{
		free_strs(builtins);
		return (1);
	}
	free_strs(builtins);
	return (0);
}

int	exec_builtin_from_child(t_msh *m, int forks)
{
	char	*name;
	char	**args;

	args = m->ct->cmds[forks]->args;
	name = args[0];
	if (s_iseq(name, "echo"))
	{
		ft_echo(m, args);
		free_all(m);
		exit(0);
	}
	else
	{
		free_all(m);
		exit(0);
	}
	return (0);
}

int	exec_builtin(t_msh *m, int forks, int *fd)
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
	}
	else if (s_iseq(name, "unset"))
		return (ft_unset(m, args));
	else if (s_iseq(name, "env"))
		return (ft_env(m, args));
	else if (s_iseq(name, "exit"))
	{
		ft_free((void **)&fd);
		return (ft_exit(m, args));
	}
	return (0);
}

int	execute_cmd(t_msh *m, int forks)
{
	t_execve	*ev;
	t_cmd		*cmd;

	cmd = m->ct->cmds[forks];
	if (is_builtin(cmd))
	{
		exec_builtin_from_child(m, forks);
	}
	else
	{
		ev = mk_execve(m, m->ct->cmds[forks]);
		if ((ev != NULL))
		{
			if (execve(ev->pathname, ev->args, *m->env) == -1)
			{
				ft_printf("%s: command not found\n", ev->args[0]);
				del_execve(ev);
				free_all(m);
				exit(1);
			}
		}
		free_all(m);
		exit(1);
	}
	return (0);
}

int	*mk_pipes(t_msh *m)
{
	int	*fd;
	int	i;

	fd = (int *)ft_calloc(sizeof(int), (m->ct->senc + 1) * 2);
	if (!fd)
		return (NULL);
	i = -1;
	while (++i < m->ct->senc + 1)
	{
		if (pipe(fd + i * 2))
		{
			perror("pipe");
			return (NULL);
		}
	}
	return (fd);
}

//WATCHOUT FOR THE RIGHT ORDER!!! Currently file > heredoc
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
		if (forks != 0 || m->ct->cmds[forks]->here)
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
		if (!(m->ct->cmds[forks]->appp))
			unlink(outfile);
		fd_open = open(outfile, O_WRONLY | O_CREAT | O_APPEND \
		* (m->ct->cmds[forks]->appp), 0666);
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
	ft_free((void **)&fd);
	if (execute_cmd(m, forks) == 1)
		return (1);
	return (0);
}

int	run_parent(t_msh *m, int *fd, int forks)
{
	int		fdin;
	char	*tmp;

	if (m->ct->cmds[0]->in && forks == 0)
	{
		fdin = open(m->ct->cmds[0]->in, O_RDONLY);
		tmp = get_next_line(fdin);
		while (tmp)
		{
			write(fd[forks * 2 + 1], tmp, ft_strlen(tmp));
			ft_free((void **)&tmp);
			tmp = get_next_line(fdin);
		}
		if (fdin)
			close(fdin);
	}
	if (m->ct->cmds[forks]->here)
	{
		ft_putstr_fd(m->ct->cmds[forks]->here, fd[forks * 2 + 1]);
	}
	close(fd[forks * 2 + 0]);
	close(fd[forks * 2 + 1]);
	return (0);
}

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

int	exec_cmds_builtin(t_msh *m)
{
	int	*fd;
	int	forks;
	int	tmp_return;

	fd = mk_pipes(m);
	if (fd == NULL)
		return (0);
	tmp_return = exec_cmds_builtin_sub(m, &forks, fd);
	if (tmp_return)
		return (tmp_return);
	exec_cmds_builtin_sub2(m, fd, forks);
	while (1)
	{
		if (wait(NULL) <= 0)
			break ;
	}
	ft_free((void **)&fd);
	return (0);
}

int	is_last_builtin(t_msh *m)
{
	int		i;
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
