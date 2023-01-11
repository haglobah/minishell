/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 21:09:45 by tpeters           #+#    #+#             */
/*   Updated: 2023/01/11 21:10:06 by tpeters          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

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
