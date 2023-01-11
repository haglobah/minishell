/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 21:17:55 by tpeters           #+#    #+#             */
/*   Updated: 2023/01/11 21:18:16 by tpeters          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

int	consists_of_only(char *token, char *chars)
{
	while (*token)
	{
		if (!char_in_set(*token, chars))
		{
			ft_printf("Not in set: %c\n", *token);
			return (0);
		}
		token++;
	}
	return (1);
}

int	s_in_s(char *s, char **slist)
{
	int	i;

	i = 0;
	while (slist[i])
	{
		if (s_iseq(s, slist[i]))
			return (1);
		i++;
	}
	return (0);
}

void	serror(char *token)
{
	ft_printf("bash: syntax error near unexpected token '%s'\n", token);
}

int	serrorm1(char *token)
{
	ft_printf("bash: syntax error near unexpected token '%s'\n", token);
	return (-1);
}

int	strslen(char **strs)
{
	int	i;

	i = -1;
	if (strs == NULL)
		return (0);
	while (strs[++i] != NULL)
		;
	return (i);
}
