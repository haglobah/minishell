/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 21:07:26 by tpeters           #+#    #+#             */
/*   Updated: 2023/01/11 21:07:49 by tpeters          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

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
