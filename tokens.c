/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 20:17:26 by bhagenlo          #+#    #+#             */
/*   Updated: 2022/11/25 15:34:49 by tpeters          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

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

//returns NULL terminated array of strings
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
