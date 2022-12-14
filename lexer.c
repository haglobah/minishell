/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 13:21:59 by bhagenlo          #+#    #+#             */
/*   Updated: 2022/12/15 00:33:32 by tpeters          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

void	add_tok(t_list **res, char *in, t_lex *l, char *place)
{
	char	*substr;

	(void)place;
	substr = ft_substr(in, l->cst, l->n - l->cst);
	if (!substr)
		return ;
	//ft_printf("Token added in %s: '%s'\n", place, substr);
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

int		read_n_app(char **line, char **in)
{
	*line = readline("heredoc>");
	if (*line == NULL)
	{
		ft_printf("^D\n");
		return (0);
	}
	int old_in_len = ft_strlen(*in);
	int	herelen = ft_strlen(*line) + old_in_len + 1;
	*in = (char *)ft_reallocpl(*in, herelen + 1, "in");
	if (!(*in))
		return (0);
	(*in)[old_in_len] = '\n';
	(*in)[herelen] = '\0';
	//nur für HEREDOCS am ende der zeile (weil appended nur und added nicht in der mitte)
	char *tmp = ft_strjoin(*in, *line);
	ft_free(*in);
	*in = tmp;
	if (!(*in))
		return (0);
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
		if (read_n_app(&line, &(in->t)) == 0)
		{
			return ;
		}
		in->here_did_realloc = true;
		l->n += ft_strlen(line) + 1;
		if (s_iseq(line, delim))
			break ;
		ft_free(line);
	}
	ft_free(line);
	add_tok(res, in->t, l, "heredoc");
}

char	*read_delim(char *in, t_lex *l)
{
	char	*delim;

	while (in[l->n] && char_in_set(in[l->n], "'\" \v\t\f\r"))
	{
		(l->n)++;
	}
	l->cst = l->n;
	while (in[l->n] && !char_in_set(in[l->n], "'\" \v\t\f\r\n"))
	{
		(l->n)++;
	}
	delim = ft_substr(in, l->cst, l->n - l->cst);
	if (!delim)
		return (NULL);
	if (char_in_set(in[l->cst - 1], "'\""))
		l->cst += -1;
	while (in[l->n] && char_in_set(in[l->n], "'\" \v\t\f\r"))
		(l->n)++;
	return (delim);
}

void	handle_pipered(t_list **res, t_in *in, t_lex *l)
{
	char	*delim;

	if (l->ctt == 1
		&& (in->t[l->cst] == '|'
			|| l->n - l->cst > 1
			|| (in->t)[l->cst] != (in->t)[l->n]))
	{
		add_tok(res, (in->t), l, "pipe&red");
		if (s_isneq(&(in->t)[l->cst], "<<", 2))
		{
			l->cst = l->n;
			delim = read_delim((in->t), l);
			handle_nlenv(res, in, l, delim);
			ft_free(delim);
		}
		l->cst = l->n;
		l->ctt = 0;
	}
}

void	handle_quotes(t_list **res, char *in, t_lex *l)
{
	while (char_in_set(in[l->n], "\'\""))
	{
		if (l->n != l->cst)
			add_tok(res, in, l, "q1");
		l->cst = l->n;
		(l->n)++;
		while (in[l->n] && in[l->cst] != in[l->n])
			(l->n)++;
		if (in[l->n])
			(l->n)++;
		add_tok(res, in, l, "q2");
		l->cst = l->n;
		l->ctt = 0;
	}
}

int	handle_vars(t_list **res, char *in, t_lex *l)
{
	if (in[l->n] == '$')
	{
		if (l->n != l->cst)
			add_tok(res, in, l, "v1");
		l->cst = l->n;
		(l->n)++;
		if (in[l->n] == '?')
		{
			(l->n)++;
		}
		else
		{
			while (in[l->n] && char_in_set(in[l->n], "abcdefghijklmnopqrstuvwxyz\
				ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789")
				&& ((l->n - l->cst == 1
						&& char_in_set(in[l->n], "abcdefghijklmnopqrstuvwxyz\
							ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
					|| l->n - l->cst != 1))
			{
				(l->n)++;
			}
		}
		add_tok(res, in, l, "v2");
		l->cst = l->n;
		return (1);
	}
	return (0);
}

int	handle_prn(t_list **res, char *in, t_lex *l)
{
	if (l->ctt != 1 && char_in_set(in[l->n], "|><\n"))
	{
		if (l->n != l->cst)
			add_tok(res, in, l, "prn");
		l->cst = l->n;
		l->ctt = 1;
		if (in[l->n])
			(l->n)++;
		return (1);
	}
	return (0);
}

int	handle_whitespace(t_list **res, char *in, t_lex *l)
{
	if (l->n != l->cst)
		add_tok(res, in, l, "ws");
	l->cst = l->n;
	if (in[l->n + 1])
	{
		l->cst = l->n + 1;
		l->n++;
		return (1);
	}
	else
	{
		l->n++;
		return (0);
	}
}

void	handle_rest(t_list **res, char *in, t_lex *l)
{
	if (l->ctt == 0)
	{
		if (l->n != l->cst)
			add_tok(res, in, l, "word");
		l->ctt = 2;
	}
	if (in[l->n])
		l->n++;
}

t_list	*lex(t_in *in)
{
	t_list	*res;
	t_lex	l;

	res = NULL;
	l = (t_lex){.cst = 0, .n = 0, .ctt = 0};
	while (1)
	{
		if (handle_nullchar(&res, in->t, &l))
			break ;
		handle_pipered(&res, in, &l);
		handle_quotes(&res, in->t, &l);
		if (handle_vars(&res, in->t, &l))
			continue ;
		if (handle_prn(&res, in->t, &l))
			continue ;
		if (char_in_set(in->t[l.n], " \v\t\f\r"))
		{
			if (handle_whitespace(&res, in->t, &l))
				continue ;
			break ;
		}
		handle_rest(&res, in->t, &l);
	}
	return (res);
}
