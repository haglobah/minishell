/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhagenlo <bhagenlo@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 17:00:48 by bhagenlo          #+#    #+#             */
/*   Updated: 2022/12/04 19:39:58 by bhagenlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

//Beware: c = 1! (pipecount one 'too high')
int	count_pipes(char **toks, int *places)
{
	int	c;
	int	i;

	i = 0;
	c = 1;
	while (toks[i])
	{
		if (s_iseq(toks[i], "|"))
		{
			places[c] = i + 1;
			free(toks[i]);
			toks[i] = NULL;
			c++;
		}
		i++;
	}
	places[c] = i;
	return (c);
}

void	put_split_to_table(t_msh *m, int pi, int *pipe_places)
{
	int	tok_count;
	int	sen_i;
	int	tok_place;

	sen_i = 0;
	tok_count = pipe_places[pi + 1] - pipe_places[pi];
	m->ct->sentences[pi] = (char **)ft_calloc(tok_count + 1, sizeof(char *));
	while (sen_i < tok_count)
	{
		tok_place = pipe_places[pi] + sen_i;
		if (m->toks[tok_place])
		{
			m->ct->sentences[pi][sen_i] = ft_strdup(m->toks[tok_place]);
		}
		sen_i++;
	}
}

int	print_sens(t_msh *m)
{
	int	i;
	int	j;

	i = 0;
	ft_printf(" ");
	while (m->ct->sentences[i])
	{
		j = 0;
		while (m->ct->sentences[i][j])
		{
			ft_printf("  %s", m->ct->sentences[i][j]);
			j++;
		}
		ft_printf("\n|");
		i++;
	}
	ft_printf("\n");
	return (1);
}

int	split_by_pipes(t_msh *m)
{
	int	i;
	int	pipecount;
	int	*pipe_places;

	pipe_places = (int *)ft_calloc(NUM_PIPES + 1, sizeof(int));
	if (!pipe_places)
		return (-2);
	pipecount = count_pipes(m->toks, pipe_places);
	m->ct->sentences = (char ***)ft_calloc(pipecount + 1, sizeof(char **));
	m->ct->senc = pipecount;
	if (!m->ct->sentences)
		return (-3);
	i = 0;
	while (i < pipecount)
	{
		put_split_to_table(m, i, pipe_places);
		i++;
	}
	free(pipe_places);
	return (pipecount);
}

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
	int	i;
	int	rc;
	char	*redirs[] = {"<", ">", "<<", ">>", NULL};

	(void)m;
	i = 0;
	rc = 0;
	while (sen[i])
	{
		if (s_in_s(sen[i], redirs))
		{
			if (!sen[i + 1])
			{
				serror(sen[i]);
				return (-1);
			}
			if (s_in_s(sen[i + 1], redirs))
			{
				serror(sen[i + 1]);
				return (-1);
			}
			rc += 2;
		}
		i++;
	}
	return (i - rc);
}

char	*parse_here(t_cmd *cmd, char *here)
{
	int	i;
	int	j;

	j = ft_strlen(here) - 2;
	while (here[j] != '\n')
	{
		j--;
	}
	i = 0;
	//is_quoted
	if (char_in_set(here[i], "'\""))
	{
		//del_delim
		i++;
		while (!char_in_set(here[i], "'\""))
			i++;
		i++;
		cmd->here_quoted = 1;
	}
	else
	{
		i += ft_strlen(here) - 2 - j;
	}
	return (ft_substr(here, i, j - i));
}

int	compute_io(t_msh *m, char **sen, t_cmd *cmd)
{
	int	i;
	int	j;

	cmd->argc = check_redirs(m, sen);
	if(cmd->argc == -1)
		return (0);
	cmd->appp = 0;
	cmd->argv = (char **)ft_calloc(cmd->argc + 1, sizeof(char *));
	cmd->args = NULL;
	i = 0;
	j = 0;
	while (sen[i])
	{
		if (s_iseq(sen[i], "<"))
		{
			cmd->in = ft_strdup(sen[i + 1]);
			i++;
		}
		else if (s_iseq(sen[i], ">"))
		{
			cmd->out = ft_strdup(sen[i + 1]);
			i++;
		}
		else if (s_iseq(sen[i], "<<"))
		{
			cmd->here = parse_here(cmd, sen[i + 1]);
			i++;
		}
		else if (s_iseq(sen[i], ">>"))
		{
			cmd->out = ft_strdup(sen[i + 1]);
			cmd->appp = 1;
			i++;
		}
		else
		{
			cmd->argv[j] = ft_strdup(sen[i]);
			j++;
		}
		i++;
	}
	return (1);
}

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
		m->ct->cmds[i] = (t_cmd *)ft_calloc(1 , sizeof(t_cmd));
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
