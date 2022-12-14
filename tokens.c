/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 20:17:26 by bhagenlo          #+#    #+#             */
/*   Updated: 2022/12/15 00:02:55 by tpeters          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

void	print_tokens(t_list *tokens)
{
	int	i;

	i = 0;
	while (tokens)
	{
		ft_printf("%d: '%s'\n", i, tokens->content);
		tokens = tokens->next;
		i++;
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

int		alloc_toks(t_list *toks, char ***tok_arr)
{
	int	i;
	int	slen;

	slen = 0;
	i = 0;
	while (toks)
	{
		slen = ft_strlen(toks->content);
		(*tok_arr)[i] = (char *)ft_calloc(slen + 1, sizeof(char));
		if (!(*tok_arr)[i])
			return (0);
		ft_strcpy((*tok_arr)[i], toks->content);
		toks = toks->next;
		i++;
	}
	return (1);
}

//returns NULL-terminated array of strings
char	**list_to_arr(t_list *toks)
{
	int		llen;
	char	**tok_arr;
	t_list	*cpy;

	llen = 0;
	cpy = toks;
	while (cpy)
	{
		llen++;
		cpy = cpy->next;
	}
	tok_arr = (char **)ft_calloc(llen + 1, sizeof(char *));
	if (!tok_arr)
		return (NULL);
	if (alloc_toks(toks, &tok_arr) == 0)
		return (NULL);
	return (tok_arr);
}
