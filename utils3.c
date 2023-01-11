/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 21:18:25 by tpeters           #+#    #+#             */
/*   Updated: 2023/01/11 21:18:46 by tpeters          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

int	free_strs(char **sp)
{
	int	i;

	if (!sp)
		return (0);
	i = -1;
	while (sp[++i] != NULL)
	{
		ft_free((void **)&sp[i]);
	}
	ft_free((void **)&sp);
	return (0);
}

int	free_strspl(char **sp, char *place)
{
	int	i;

	(void)place;
	if (!sp)
		return (0);
	i = -1;
	while (sp[++i] != NULL)
	{
		ft_free((void **)&sp[i]);
	}
	ft_free((void **)&sp);
	return (0);
}

void	*free_strsv(char **sp)
{
	free_strs(sp);
	return (NULL);
}

bool	free_strsb(char **sp)
{
	free_strs(sp);
	return (false);
}

void	prints(char **slist)
{
	int	i;

	i = -1;
	while (slist[++i])
	{
		ft_printf("'%s'\n", slist[i]);
	}
}
