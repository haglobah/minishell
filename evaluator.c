/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 11:26:40 by bhagenlo          #+#    #+#             */
/*   Updated: 2023/01/11 20:57:22 by tpeters          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

char	*get_var(char **sp, int dollar)
{
	int	i;

	i = 0;
	while (char_in_set((*sp)[++i + dollar],
		"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"))
		;
	if (i == 1)
		return (NULL);
	return (ft_substr(*sp, dollar + 1, i));
}

void	replace_string_at(char **sp, int dollar, char *content, int vlen)
{
	t_rep	t;

	t = (t_rep){-1, -1, 0, 0, 0, NULL};
	t.newlen = ft_strlen(*sp) + ft_strlen(content) - (vlen + 1);
	t.new_arg = ft_calloc(sizeof(char), t.newlen + 1);
	while (++t.i < dollar)
	{
		t.new_arg[t.i] = (*sp)[t.i];
	}
	while (++t.j < (int)ft_strlen(content))
	{
		t.new_arg[t.i + t.j] = content[t.j];
	}
	t.k = t.i + t.j;
	t.z = t.i + vlen + 1;
	while ((*sp)[t.z])
	{
		t.new_arg[t.k] = (*sp)[t.z];
		t.k++;
		t.z++;
	}
	ft_free((void **)&(*sp));
	*sp = t.new_arg;
}

void	find_replace(t_msh *m, char **sp, int dollar)
{
	int		vlen;
	char	*content;
	char	*varname;

	vlen = 0;
	content = NULL;
	varname = get_var(sp, dollar);
	if (varname != NULL)
	{
		vlen = ft_strlen(varname);
		content = ft_getenv(m->env, varname);
		if (content != NULL)
		{
			replace_string_at(sp, dollar, content, vlen);
			ft_free((void **)&content);
		}
		else
		{
			replace_string_at(sp, dollar, "", vlen);
		}
		ft_free((void **)&varname);
	}
}

void	insert_rv(t_msh *m, char **sp, int dollar)
{
	char	*str;

	str = ft_itoa(*(m->rv));
	replace_string_at(sp, dollar, str, 1);
	ft_free((void **)&str);
}

int	rm_quotes(char **sp, int here_quoted)
{
	int		is_quoted;
	char	*tmp;

	if (here_quoted)
		return (1);
	is_quoted = 0;
	if (!char_in_set((*sp)[0], "'\""))
		return (is_quoted);
	if ((*sp)[0] == '\'')
		is_quoted = 1;
	tmp = *sp;
	*sp = ft_substr(*sp, 1, ft_strlen(*sp) - 2);
	ft_free((void **)&tmp);
	return (is_quoted);
}
