/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/17 10:23:18 by tpeters           #+#    #+#             */
/*   Updated: 2022/09/17 17:47:11 by tpeters          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>



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
					&& ((n - curr_start == 1 && char_in_set(in[n], "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")) || n - curr_start != 1))
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

char	**parse(t_list *in)
{
	//make AST
	while(in)
	{
		ft_printf("%s%%\n", in->content);
		in = in->next;
	}
}

void	interpret(char **in)
{
	//run AST
}

void	msh_loop(void)
{
	char	*t;
	while(1)
	{
		t = readline("φ ");
		add_history(t);
		if (t)
			interpret(parse(lex(t)));
		free(t);
	}
}

int	main(void)
{
  msh_loop();
  return (0);
}
