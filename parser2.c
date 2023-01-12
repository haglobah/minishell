/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 21:15:04 by tpeters           #+#    #+#             */
/*   Updated: 2023/01/12 15:40:20 by tpeters          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

int	pipe_checkup(t_msh *m)
{
	int	i;

	i = 0;
	while (m->ct->sentences[i])
	{
		if (!m->ct->sentences[i][0])
		{
			serror("|");
			return (0);
		}
		i++;
	}
	return (1);
}

int	check_redirs(t_msh *m, char **sen)
{
	int		i;
	int		rc;
	char	**redirs;

	(void)m;
	redirs = mk_strlist(4, "<", ">", "<<", ">>");
	if (!redirs)
		return (0);
	i = 0;
	rc = 0;
	while (sen[i])
	{
		if (s_in_s(sen[i], redirs))
		{
			if (!sen[i + 1])
				return (serrorm1(sen[i]));
			if (s_in_s(sen[i + 1], redirs))
				return (serrorm1(sen[i + 1]));
			rc += 2;
		}
		i++;
	}
	free_strs(redirs);
	return (i - rc);
}

char	*parse_here(t_cmd *cmd, char *here)
{
	int	i;
	int	j;

	if (here == NULL)
		return (NULL);
	j = ft_strlen(here) - 2;
	while (here[j] != '\n')
		j--;
	i = 0;
	if (char_in_set(here[i], "'\""))
	{
		i++;
		while (!char_in_set(here[i], "'\""))
			i++;
		cmd->here_quoted = 1;
	}
	else
	{
		while (here[i] != '\n')
			i++;
	}
	i++;
	if (j <= i)
		return (ft_strdup(""));
	return (ft_substr(here, i, j - i));
}

void	fill_io(char **sen, t_cmd *cmd, int *i, int *j)
{
	if (s_iseq(sen[(*i)], "<"))
		cmd->in = ft_strdup(sen[++(*i)]);
	else if (s_iseq(sen[(*i)], ">"))
		cmd->out = ft_strdup(sen[++(*i)]);
	else if (s_iseq(sen[(*i)], "<<"))
		cmd->here = parse_here(cmd, sen[++(*i)]);
	else if (s_iseq(sen[(*i)], ">>"))
	{
		cmd->out = ft_strdup(sen[++(*i)]);
		cmd->appp = 1;
	}
	else
		cmd->argv[++(*j)] = ft_strdup(sen[(*i)]);
}

int	compute_io(t_msh *m, char **sen, t_cmd *cmd)
{
	int	i;
	int	j;

	cmd->argc = check_redirs(m, sen);
	if (cmd->argc == -1)
		return (0);
	cmd->appp = 0;
	cmd->argv = (char **)ft_calloc(cmd->argc + 1, sizeof(char *));
	cmd->args = NULL;
	i = -1;
	j = -1;
	while (sen[++i])
	{
		fill_io(sen, cmd, &i, &j);
	}
	return (1);
}
