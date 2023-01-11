/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 20:59:59 by tpeters           #+#    #+#             */
/*   Updated: 2023/01/11 21:00:49 by tpeters          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

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
