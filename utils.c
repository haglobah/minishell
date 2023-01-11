/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 20:11:20 by bhagenlo          #+#    #+#             */
/*   Updated: 2023/01/11 21:19:10 by tpeters          ###   ########.fr       */
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
		return (ft_calloc(1, size));
	if (!size)
		return (ptr);
	new_ptr = malloc(size);
	if (new_ptr == NULL)
		return (NULL);
	ft_memcpy(new_ptr, ptr, size);
	ft_free((void **)&ptr);
	return (new_ptr);
}

// ft_printf("%s: freeing a NULL pointer/a pointer twice.\n");
void	ft_free(void **ptr)
{
	if (*ptr != NULL)
	{
		free(*ptr);
		*ptr = NULL;
	}
	else
	{
	}
}

int	s_isneq(char *s1, char *s2, int n)
{
	if (!s1)
	{
		ft_printf("There is no token.\n");
		return (0);
	}
	return (!ft_strncmp(s1, s2, n));
}

int	s_iseq(char *s1, char *s2)
{
	return (!ft_strcmp(s1, s2));
}
