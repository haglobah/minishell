/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 09:39:54 by bhagenlo          #+#    #+#             */
/*   Updated: 2023/01/11 21:07:45 by tpeters          ###   ########.fr       */
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
