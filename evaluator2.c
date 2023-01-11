/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluator2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 20:57:03 by tpeters           #+#    #+#             */
/*   Updated: 2023/01/11 20:57:29 by tpeters          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

void	swap_in_vars(t_msh *m, char **sp, int here_quoted)
{
	int	i;

	if (*sp == NULL)
		return ;
	if (rm_quotes(sp, here_quoted))
		return ;
	i = -1;
	while ((*sp)[++i])
	{
		if ((*sp)[i] == '$')
		{
			if ((*sp)[i + 1] == '?')
				insert_rv(m, sp, i);
			else
				find_replace(m, sp, i);
		}
	}
}

char	**con_args_wrapped(t_cmd *cmd, char **args)
{
	int		i;
	int		j;

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

char	**cons_args(t_cmd *cmd)
{
	int		i;
	int		empty_count;
	char	**args;

	empty_count = 0;
	i = -1;
	while (++i < cmd->argc)
	{
		if (s_iseq(cmd->argv[i], ""))
			empty_count += 1;
	}
	if (i == 0)
		return (NULL);
	args = ft_calloc(sizeof(char *), (i - empty_count + 1));
	if (args == NULL)
		return (NULL);
	return (con_args_wrapped(cmd, args));
}

void	loop_through_cmds(t_msh *m)
{
	int		i;
	int		j;
	t_cmd	**cmds;

	cmds = m->ct->cmds;
	i = -1;
	while (cmds[++i] != NULL)
	{
		j = -1;
		while (++j < cmds[i]->argc)
		{
			swap_in_vars(m, &cmds[i]->argv[j], cmds[i]->here_quoted);
		}
		swap_in_vars(m, &cmds[i]->in, cmds[i]->here_quoted);
		swap_in_vars(m, &cmds[i]->out, cmds[i]->here_quoted);
		cmds[i]->args = cons_args(cmds[i]);
	}
}

int	evaluate(t_msh *m)
{
	loop_through_cmds(m);
	return (0);
}
