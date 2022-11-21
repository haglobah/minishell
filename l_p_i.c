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

void	add_tok(t_list **res, char *in, int *cst, int *n, char *place)
{
	ft_printf(" Tok: '%s', added in: %s;\n", ft_substr(in, *cst, *n - *cst), place);
	ft_lstadd_back(res, ft_lstnew(ft_substr(in, *cst, *n - *cst)));
}

int	handle_nullchar(t_list **res, char *in, int *cst, int *n)
{
	if (in[*n]  == '\0')
	{
		if (*n != *cst)
			add_tok(res, in, cst, n, "null");
		return (1);
	}
	return (0);
}

void	read_n_app(char **line, char **in, int *n)
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

void	handle_nlenv(t_list **res, char *in, int *cst, int *n, int *cttype, char *delim)
{
	char	*line;

//	insert_nl(&in);
	read_n_app(&line, &in, n);
	(*n) += ft_strlen(line);
	while (!s_iseq(line, delim))
	{
		read_n_app(&line, &in, n);
		(*n) += ft_strlen(line);
	}
	if(s_iseq(line, delim))
	{
		add_tok(res, in, cst, n, "heredoc");
	}
	//free(line);
}

char	*read_delim(char *in, int *cst, int *n)
{
	char	*delim;

	while (in[*n] && char_in_set(in[*n], "'\" \v\t\f\r"))
	{
		(*n)++;
	}
	*cst = *n;
	while (in[*n] && !char_in_set(in[*n], "'\" \v\t\f\r\n"))
	{
		(*n)++;
	}
	delim = ft_substr(in, *cst, *n - *cst);
	delim = ft_realloc(delim, ft_strlen(delim) + 2);
	delim[ft_strlen(delim)] = '\n';
	delim[ft_strlen(delim) + 1] = '\0';
	if (char_in_set(in[*cst - 1], "'\""))
		*cst += -1;
	while (in[*n] && char_in_set(in[*n], "'\" \v\t\f\r"))
		(*n)++;
	return (delim);
}

void	handle_pipered(t_list **res, char *in, int *cst, int *n, int *cttype)
{
	char	*delim;

	if (*cttype == 1
		&& (in[*cst] == '|'
			|| *n - *cst > 1
			|| in[*cst] != in[*n]))
	{
		add_tok(res, in, cst, n, "pipe&red");
		if (s_isneq(&in[*cst], "<<", 2))
		{
			*cst = *n;
//			ft_printf("A HERE. This one: ");
			// is quoted?
			delim = read_delim(in, cst, n);
			ft_printf("'%s'\n", delim);
			handle_nlenv(res, in, cst, n, cttype, delim);
		}
		*cst = *n;
		*cttype = 0;
	}
}

void	handle_quotes(t_list **res, char *in, int *cst, int *n, int *cttype)
{
	while (char_in_set(in[*n], "\'\""))
	{
		if (*n != *cst)
			add_tok(res, in, cst, n, "q1");
		*cst = *n;
		(*n)++;
		while (in[*n] && in[*cst] != in[*n])
			(*n)++;
		if (in[*n])
			(*n)++;
		add_tok(res, in, cst, n, "q2");
		*cst = *n;
		*cttype = 0;
	}
}

int	handle_vars(t_list **res, char *in, int *cst, int *n, int *cttype)
{
	if (in[*n] == '$')
	{
		if (*n != *cst)
			add_tok(res, in, cst, n, "v1");
		*cst = *n;
		(*n)++;
		if (in[*n] == '?')
		{
			(*n)++;
		}
		else
		{
			while (in[*n] && char_in_set(in[*n], "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789")
				&& ((*n - *cst == 1
					 && char_in_set(in[*n], "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"))
					|| *n - *cst != 1))
			{
				(*n)++;
			}
		}
		add_tok(res, in, cst, n, "v2");
		*cst = *n;
		return (1);
	}
	return (0);
}

int	handle_prn(t_list **res, char *in, int *cst, int *n, int *cttype)
{
	if (*cttype != 1 && char_in_set(in[*n], "|><\n"))
	{
		if (*n != *cst)
			add_tok(res, in, cst, n, "prn");
		*cst = *n;
		*cttype = 1;
		if (in[*n])
			(*n)++;
		return (1);
	}
	return (0);
}

t_list	*lex(char *in)
{
	int	curr_start = 0;
	int	n = 0;
	int	curr_token_type = 0; // 0 default ; 1 operator ; 2 word
	t_list	*res = NULL;
	while (1)
	{
		if (handle_nullchar(&res, in, &curr_start, &n))
			break ;
		handle_pipered(&res, in, &curr_start, &n, &curr_token_type);
		handle_quotes(&res, in, &curr_start, &n, &curr_token_type);
		if (handle_vars(&res, in, &curr_start, &n, &curr_token_type))
			continue ;
		if (handle_prn(&res, in, &curr_start, &n, &curr_token_type))
			continue ;
		if (char_in_set(in[n], " \v\t\f\r"))
		{
			if (n != curr_start)
				add_tok(&res, in, &curr_start, &n, "ws");
			curr_start = n;
			if (in[n + 1])
			{
				curr_start = n + 1;
				n++;
				continue ;
			}
			else
			{
				n++;
				break;
			}
		}
		if (curr_token_type == 0)
		{
			if (n != curr_start)
				add_tok(&res, in, &curr_start, &n, "word");
			curr_token_type = 2;
		}
		if (in[n])
			n++;
	}
	return (res);
}

void	print_tokens(t_list *tokens)
{
	while(tokens)
	{
		ft_printf("%s%%\n", tokens->content);
		tokens = tokens->next;
	}
}

void	print_tokarr(char **toks)
{
	if (!toks)
		return ;
	ft_printf("All the tokens: ");
	while (*toks)
	{
		ft_printf("'%s' ", *toks);
		toks++;
	}
	ft_printf("\n");
}

void	ft_strcpy(char *dst, char *src)
{
	while (*src)
	{
		*dst++ = *src++;
	}
}

char	**list_to_arr(t_list *toks)
{
	int	slen;
	int	llen;
	char	**tok_arr;
	t_list	*cpy;
	int	i;

	llen = 0;
	cpy = toks;
	while (cpy)
	{
		llen++;
		cpy = cpy->next;
	}
//	ft_printf("toks: %i\n", llen);
	tok_arr = (char **)ft_calloc(llen + 1, sizeof(char *));
	if (!tok_arr)
		return (NULL);
	slen = 0;
	i = 0;
	while (toks)
	{
		slen = ft_strlen(toks->content);
		tok_arr[i] = (char *)ft_calloc(slen + 1, sizeof(char));
		if (!(*tok_arr))
			return (NULL);
		ft_strcpy(tok_arr[i], toks->content);
//		ft_printf("Copied %s\n", tok_arr[i]);
		toks = toks->next;
		i++;
	}
	return (tok_arr);
}

void	interpret(t_list *parse_tree)
{
	//run AST
}

t_ct	*mk_ct()
{
	t_ct	*ct;

	ct = (t_ct *)ft_calloc(1, sizeof(t_ct));
	if (!ct)
		return (NULL);
	ct->cmds = NULL;
	ct->senc = -1;
	return (ct);
}

t_msh	*mk_msh(char **toks)
{
	t_msh	*msh;

	msh = (t_msh *)ft_calloc(1, sizeof(t_msh));
	if (!msh)
		return (NULL);
	msh->toks = toks;
	msh->ct = mk_ct();
	return (msh);
}

void	del_msh(t_msh *m)
{
	//free_toks(m->toks);
	int	i;
	int	j;

	i = 0;
	while (m->toks[i])
	{
		free(m->toks[i]);
		i++;
	}
	//del_ct(m->ct)
	free(m);
}


// msh_loop = execute . evaluate . parse . tokenize
void	msh_loop(void)
{
	char	*t;
	char	**toks;
	t_msh	*m;

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
				//ft_printf("Good Command!\n");
				//print_tokens(parse_tree);
				//interpret();
			}
			else
			{
				ft_printf("Bad Command!\n");
			}
		}
		free(t);
		del_msh(m);
	}
}

