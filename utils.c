/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhagenlo <bhagenlo@student.42heil...>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 20:11:20 by bhagenlo          #+#    #+#             */
/*   Updated: 2022/11/21 20:11:20 by bhagenlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

void	ft_strcpy(char *dst, char *src)
{
	while (*src)
	{
		*dst++ = *src++;
	}
}

void	*ft_realloc(void *ptr, size_t size)
{
	void	*new_ptr;

	if (ptr == NULL)
		return (malloc(size));
	if (!size)
		return (ptr);
	new_ptr = malloc(size);
	ft_memcpy(new_ptr, ptr, size);
	free(ptr);
	return (new_ptr);
}

int	s_isneq(char *s1, char *s2, int n)
{
	if (!s1)
		ft_printf("There is no token.\n");
	return (!ft_strncmp(s1, s2, n));
}

int	s_iseq(char *s1, char *s2)
{
	return (!ft_strcmp(s1, s2));
}

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
		if(s_iseq(s, slist[i]))
			return (1);
		i++;
	}
	return (0);
}

void	serror(char *token)
{
	ft_printf("bash: syntax error near unexpected token '%s'\n", token);
}


void	free_strs(char **sp)
{
	int	i;

	i = -1;
	while (sp[++i] != NULL)
	{
		free(sp[i]);
	}
	free(sp);
	return ;
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
