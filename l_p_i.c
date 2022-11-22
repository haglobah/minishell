/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l_p_i.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhagenlo <bhagenlo@student.42heil...>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 13:21:59 by bhagenlo          #+#    #+#             */
/*   Updated: 2022/09/21 13:21:59 by bhagenlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

void	add_tok(t_list **res, char *in, t_lex *l, char *place)
{
	/* ft_printf(" Tok: '%s', added in: %s;\n", ft_substr(in, l->cst, l->n - l->cst), place); */
	ft_lstadd_back(res, ft_lstnew(ft_substr(in, l->cst, l->n - l->cst)));
}

int	handle_nullchar(t_list **res, char *in, t_lex *l)
{
	if (in[l->n]  == '\0')
	{
		if (l->n != l->cst)
			add_tok(res, in, l, "null");
		return (1);
	}
	return (0);
}

void	read_n_app(char **line, char **in)
{
	*line = get_next_line(0);
	*in = (char *)ft_realloc(*in, ft_strlen(*line) + ft_strlen(*in) + 1);
	ft_strlcat(*in, *line, ft_strlen(*line) + ft_strlen(*in) + 1);
//	ft_printf("content: '%s'\n", *in);
	ft_printf("linelen: %i\n", ft_strlen(*line));
}

void	insert_nl(char **in)
{
	*in = ft_realloc(*in, ft_strlen(*in) + 3);
	*in[ft_strlen(*in)] = '\n';
	*in[ft_strlen(*in) + 1] = '\n';
	*in[ft_strlen(*in) + 2] = '\0';
	ft_printf("in: %s", *in);
}

void	handle_nlenv(t_list **res, char *in, t_lex *l, char *delim)
{
	char	*line;

//	insert_nl(&in);
	read_n_app(&line, &in);
	l->n += ft_strlen(line);
	while (!s_iseq(line, delim))
	{
		read_n_app(&line, &in);
		l->n += ft_strlen(line);
	}
	if(s_iseq(line, delim))
	{
		add_tok(res, in, l, "heredoc");
	}
	//free(line);
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
	delim = ft_realloc(delim, ft_strlen(delim) + 2);
	delim[ft_strlen(delim)] = '\n';
	delim[ft_strlen(delim) + 1] = '\0';
	if (char_in_set(in[l->cst - 1], "'\""))
		l->cst += -1;
	while (in[l->n] && char_in_set(in[l->n], "'\" \v\t\f\r"))
		(l->n)++;
	return (delim);
}

void	handle_pipered(t_list **res, char *in, t_lex *l)
{
	char	*delim;

	if (l->ctt == 1
		&& (in[l->cst] == '|'
			|| l->n - l->cst > 1
			|| in[l->cst] != in[l->n]))
	{
		add_tok(res, in, l, "pipe&red");
		if (s_isneq(&in[l->cst], "<<", 2))
		{
			l->cst = l->n;
//			ft_printf("A HERE. This one: ");
			// is quoted?
			delim = read_delim(in, l);
			ft_printf("'%s'\n", delim);
			handle_nlenv(res, in, l, delim);
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
			while (in[l->n] && char_in_set(in[l->n], "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789")
				&& ((l->n - l->cst == 1
					 && char_in_set(in[l->n], "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"))
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

t_list	*lex(char *in)
{
	t_list	*res = NULL;
	t_lex	l;

	l = (t_lex){.cst = 0, .n = 0, .ctt = 0};
	while (1)
	{
		if (handle_nullchar(&res, in, &l))
			break ;
		handle_pipered(&res, in, &l);
		handle_quotes(&res, in, &l);
		if (handle_vars(&res, in, &l))
			continue ;
		if (handle_prn(&res, in, &l))
			continue ;
		if (char_in_set(in[l.n], " \v\t\f\r"))
		{
			if (l.n != l.cst)
				add_tok(&res, in, &l, "ws");
			l.cst = l.n;
			if (in[l.n + 1])
			{
				l.cst = l.n + 1;
				l.n++;
				continue ;
			}
			else
			{
				l.n++;
				break;
			}
		}
		if (l.ctt == 0)
		{
			if (l.n != l.cst)
				add_tok(&res, in, &l, "word");
			l.ctt = 2;
		}
		if (in[l.n])
			l.n++;
	}
	return (res);
}

void	interpret(t_msh *m)
{
	//run AST
	(void)m;
}

void	prints(char **slist)
{
	int	i;

	i = -1;
	while (slist[++i])
	{
		ft_printf("%i: %s\n", i, slist[i]);
	}
}

// msh_loop = execute . evaluate . parse . tokenize
void	msh_loop(void)
{
	char	*t;
	char	**toks;
	t_msh	*m;
	extern char **environ;

	while(1)
	{ // is_atty ? readline : gnl
		t = readline("φ ");
		add_history(t);
		//	ft_printf("\n");
		if (t)
		{
			toks = list_to_arr(lex(t));
			m = mk_msh(toks);
			if (parse_msh(m))
			{
				evaluate(m);
				//prints(environ);
				//print_tokens(parse_tree);
				//interpret();
			}
			else
			{
				/* ft_printf("Bad Command!\n"); */
			}
		}
		free(t);
		del_msh(m);
	}
}

