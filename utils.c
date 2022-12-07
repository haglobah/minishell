/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhagenlo <bhagenlo@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 20:11:20 by bhagenlo          #+#    #+#             */
/*   Updated: 2022/12/07 14:13:02 by bhagenlo         ###   ########.fr       */
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

void	*ft_reallocpl(void *ptr, size_t size, char *place)
{
	void	*new_ptr;

	if (ptr == NULL)
		return (malloc(size));
	if (!size)
		return (ptr);
	new_ptr = malloc(size); //NOT PROTECTED
	ft_memcpy(new_ptr, ptr, size);
	ft_freepl(ptr, place);
	return (new_ptr);
}
void	*ft_realloc(void *ptr, size_t size)
{
	void	*new_ptr;

	if (ptr == NULL)
		return (malloc(size));
	if (!size)
		return (ptr);
	new_ptr = malloc(size); //NOT PROTECTED
	ft_memcpy(new_ptr, ptr, size);
	ft_free(ptr);
	return (new_ptr);
}

void	ft_freepl(void *ptr, char *place)
{
	if (ptr != NULL)
	{
		(void)place;
		free(ptr);
		ptr = NULL;
	}
	else
	{
		// ft_printf("%s: freeing a NULL pointer/a pointer twice.\n");
	}
}

void	ft_free(void *ptr)
{
	if (ptr != NULL)
	{
		free(ptr);
		ptr = NULL;
	}
	else
	{
		// ft_printf("%s: freeing a NULL pointer/a pointer twice.\n");
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

int	free_strs(char **sp)
{
	int	i;

	i = -1;
	while (sp[++i] != NULL)
	{
		ft_free(sp[i]);
	}
	ft_free(sp);
	return (0);
}

int	free_strspl(char **sp, char *place)
{
	int	i;

	i = -1;
	while (sp[++i] != NULL)
	{
		ft_freepl(sp[i], place);
	}
	ft_freepl(sp, place);
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

void	printns(char **slist)
{
	int	i;

	i = -1;
	while (slist[++i])
	{
		ft_printf("%i: %s\n", i, slist[i]);
	}
}

bool	ft_parse_int(const char *s, int *loc)
{
	int			i;
	long long	num;
	int			neg;

	num = 0;
	i = 0;
	neg = (s[i] == '-');
	i += neg;
	while (s[i] != '\0' && ft_isdigit(s[i]))
	{
		num = (num * 10) + (s[i] - '0');
		if (num > (((long long)INT_MAX) + neg))
			return (false);
		i++;
	}
	*loc = (int)(num * -((2 * neg) - 1));
	return (i > neg && s[i] == '\0');
}

char	**mk_strlist(int argc, ...)
{
	t_strlst	t;

	t.lst = ft_calloc(argc + 1, sizeof(char *));
	if (!t.lst)
		return (NULL);
	va_start(t.argp, argc);
	t.i = 0;
	while (t.i < argc)
	{
		t.str_to_add = va_arg(t.argp, char *);
		t.lst[t.i] = ft_strdup(t.str_to_add);
		if (!t.lst[t.i])
		{
			while (t.i)
			{
				t.i--;
				ft_free(t.lst[t.i]);
				ft_free(t.lst);
				return (NULL);
			}
		}
		t.i++;
	}
	va_end(t.argp);
	return (t.lst);
}
