/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 21:08:44 by tpeters           #+#    #+#             */
/*   Updated: 2023/01/11 21:09:18 by tpeters          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

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
			ft_free((void **)&delim);
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
