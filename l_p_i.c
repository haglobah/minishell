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
		}
		if (char_in_set(in[n], "|><\n"))
		{
			ft_lstadd_back(&res, ft_lstnew(ft_substr(in, curr_start, n - curr_start)));
			curr_start = n;
			curr_token_type = 1;
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
		if (curr_token_type == 2)
		{
			//?
		}
		else
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

void	interpret(t_list *parse_tree)
{
	//run AST
}

void	msh_loop(void)
{
	char	*t;
	t_list	*parse_tree;

	while(1)
	{
		t = readline("φ ");
		add_history(t);
		if (t)
		{
			parse_tree = lex(t);
			if (parse(parse_tree))
			{
				ft_printf("Good Command!\n");
				print_tokens(parse_tree);
				interpret(parse_tree);
			}
			else
			{
				ft_printf("Bad Command!\n");
			}
		}
		free(t);
	}
}

