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

t_list	*lex(char *in)
{
	int	curr_start = 0;
	int	n = 0;
	int	curr_token_type = 0; // 0 default ; 1 operator ; 2 word
	t_list	*res = NULL;
	while (1)
	{
		if (in[n] == '\0')
		{
			if (n != curr_start)
				ft_lstadd_back(&res, ft_lstnew(ft_substr(in, curr_start, n - curr_start)));
			break ;
		}
		if (curr_token_type == 1 && (in[curr_start] == '|' || n - curr_start > 1 || in[curr_start] != in[n]))
		{
			ft_lstadd_back(&res, ft_lstnew(ft_substr(in, curr_start, n - curr_start)));
			curr_start = n;
			curr_token_type = 0;
		}
		while (char_in_set(in[n], "\'\""))
		{
			if (n != curr_start)
				ft_lstadd_back(&res, ft_lstnew(ft_substr(in, curr_start, n - curr_start)));
			curr_start = n;
			n++;
			while (in[n] && in[curr_start] != in[n])
				n++;
			if (in[n])
				n++;
			ft_lstadd_back(&res, ft_lstnew(ft_substr(in, curr_start, n - curr_start)));
			curr_start = n;
			curr_token_type = 0;
		}
		if (in[n] == '$')
		{
			if (n != curr_start)
				ft_lstadd_back(&res, ft_lstnew(ft_substr(in, curr_start, n - curr_start)));
			curr_start = n;
			n++;
			if (in[n] == '?')
			{
				n++;
			}
			else
			{
				while (in[n] && char_in_set(in[n], "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789")
					&& ((n - curr_start == 1
						 && char_in_set(in[n], "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"))
						|| n - curr_start != 1))
				{
					n++;
				}
			}
			ft_lstadd_back(&res, ft_lstnew(ft_substr(in, curr_start, n - curr_start)));
			curr_start = n;
			continue ;
		}
		if (curr_token_type != 1 && char_in_set(in[n], "|><\n"))
		{
			if (n != curr_start)
				ft_lstadd_back(&res, ft_lstnew(ft_substr(in, curr_start, n - curr_start)));
			curr_start = n;
			curr_token_type = 1;
			if (in[n])
				n++;
			continue ;
		}
		if (char_in_set(in[n], " \v\t\f\r"))
		{
			if (n != curr_start)
				ft_lstadd_back(&res, ft_lstnew(ft_substr(in, curr_start, n - curr_start)));
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
				ft_lstadd_back(&res, ft_lstnew(ft_substr(in, curr_start, n - curr_start)));
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
		ft_printf("\'%s\', ", *toks);
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
	{
		t = readline("φ ");
		add_history(t);
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

