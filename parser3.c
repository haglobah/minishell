/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 21:15:39 by tpeters           #+#    #+#             */
/*   Updated: 2023/01/11 21:16:09 by tpeters          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

int	printcmd(t_cmd *cmd)
{
	int	i;

	ft_printf(" argc: %i\n", cmd->argc);
	ft_printf(" argv: ");
	i = 0;
	while (cmd->argv[i])
		ft_printf("'%s' ", cmd->argv[i++]);
	ft_printf(" args: ");
	if (cmd->args != NULL)
		prints(cmd->args);
	else
		ft_printf("(null)");
	ft_printf(" in: %s\n", cmd->in);
	ft_printf(" here: %s\n", cmd->here);
	ft_printf(" out: %s\n", cmd->out);
	ft_printf(" here_quoted: %i\n", cmd->here_quoted);
	return (i);
}

int	check_for_quotes(char **sen)
{
	int	i;

	if (sen[0] == NULL)
		return (1);
	i = 0;
	while (sen[i])
		i++;
	i--;
	if (char_in_set(sen[i][0], "'\""))
	{
		if ((ft_strlen(sen[i]) != 1)
			&& char_in_set(sen[i][ft_strlen(sen[i]) - 1], "'\""))
		{
			return (1);
		}
		else
		{
			serror(sen[i]);
			return (0);
		}
	}
	return (1);
}

int	is_only_dollar(char **sen)
{
	int	i;

	if (sen[0] == NULL)
		return (0);
	i = -1;
	while (sen[++i])
	{
		if (s_iseq(sen[i], "$"))
		{
			serror(sen[i]);
			return (1);
		}
	}
	return (0);
}

int	sens2cmds(t_msh *m)
{
	int	i;

	m->ct->cmds = (t_cmd **)ft_calloc(m->ct->senc + 1, sizeof(t_cmd *));
	i = 0;
	while (m->ct->sentences[i])
	{
		m->ct->cmds[i] = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
		if (!check_for_quotes(m->ct->sentences[i]))
			return (0);
		if (is_only_dollar(m->ct->sentences[i]))
			return (0);
		if (!compute_io(m, m->ct->sentences[i], m->ct->cmds[i]))
			return (0);
		i++;
	}
	return (1);
}

int	parse_msh(t_msh *m)
{
	int	pc;

	pc = split_by_pipes(m);
	if (pc > 1 && !pipe_checkup(m))
		return (0);
	if (!sens2cmds(m))
		return (0);
	return (1);
}
