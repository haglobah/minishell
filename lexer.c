/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 13:21:59 by bhagenlo          #+#    #+#             */
/*   Updated: 2023/01/11 21:10:04 by tpeters          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

//ft_printf("Token added in %s: '%s'\n", place, substr);
void	add_tok(t_list **res, char *in, t_lex *l, char *place)
{
	char	*substr;

	(void)place;
	substr = ft_substr(in, l->cst, l->n - l->cst);
	if (!substr)
		return ;
	ft_lstadd_back(res, ft_lstnew(substr));
}

int	handle_nullchar(t_list **res, char *in, t_lex *l)
{
	if (in[l->n] == '\0')
	{
		if (l->n != l->cst)
			add_tok(res, in, l, "null");
		return (1);
	}
	return (0);
}

//IS 'a<<D        | b<<F' => TEST2:'a<<D        ' a problem?
int	read_n_app(char **line, char **in, t_lex *l)
{
	char	*tmp;
	int		offset;
	int		herelen;

	*line = readline("heredoc>");
	if (*line == NULL)
	{
		ft_printf("^D\n");
		return (0);
	}
	herelen = ft_strlen(*line) + ft_strlen(*in) + 1;
	tmp = (char *)ft_calloc(herelen + 1, sizeof(char));
	if (!tmp)
		return (0);
	offset = 0;
	ft_memcpy(tmp, *in, l->n - offset);
	tmp[ft_strlen(tmp)] = '\n';
	ft_memcpy(tmp + ft_strlen(tmp), *line, ft_strlen(*line));
	ft_memcpy(
		tmp + ft_strlen(tmp),
		*in + l->n - offset,
		ft_strlen(*in) - (l->n - offset));
	ft_free((void **)&(*in));
	*in = tmp;
	return (1);
}

void	insert_nl(char **in)
{
	*in = ft_realloc(*in, ft_strlen(*in) + 3);
	*in[ft_strlen(*in)] = '\n';
	*in[ft_strlen(*in) + 1] = '\n';
	*in[ft_strlen(*in) + 2] = '\0';
	ft_printf("in: %s", *in);
}

void	handle_nlenv(t_list **res, t_in *in, t_lex *l, char *delim)
{
	char	*line;

	while (true)
	{
		if (read_n_app(&line, &(in->t), l) == 0)
		{
			return ;
		}
		in->here_did_realloc = true;
		l->n += ft_strlen(line) + 1;
		if (s_iseq(line, delim))
			break ;
		ft_free((void **)&line);
	}
	ft_free((void **)&line);
	add_tok(res, in->t, l, "heredoc");
}
