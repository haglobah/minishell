/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 21:18:52 by tpeters           #+#    #+#             */
/*   Updated: 2023/01/11 21:19:14 by tpeters          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

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
				ft_free((void **)&t.lst[t.i]);
				ft_free((void **)&t.lst);
				return (NULL);
			}
		}
		t.i++;
	}
	va_end(t.argp);
	return (t.lst);
}
