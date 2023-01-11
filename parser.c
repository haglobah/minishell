/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 17:00:48 by bhagenlo          #+#    #+#             */
/*   Updated: 2023/01/11 21:16:03 by tpeters          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

//Beware: c = 1! (pipecount one 'too high')
void	index_pipes(char **toks, int *places)
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
			c++;
		}
		i++;
	}
	places[c] = i;
}

static int	count_pipes(char **toks)
{
	int	c;
	int	i;

	i = 0;
	c = 1;
	while (toks[i])
	{
		if (s_iseq(toks[i], "|"))
		{
			c++;
		}
		i++;
	}
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
			if (!s_iseq(m->toks[tok_place], "|"))
			{
				m->ct->sentences[pi][sen_i] = ft_strdup(m->toks[tok_place]);
			}
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

	pipecount = count_pipes(m->toks);
	pipe_places = (int *)ft_calloc(pipecount + 1, sizeof(int));
	if (!pipe_places)
		return (-2);
	index_pipes(m->toks, pipe_places);
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
	ft_free((void **)&pipe_places);
	return (pipecount);
}
