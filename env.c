/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhagenlo <bhagenlo@student.42heil...>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 16:31:21 by bhagenlo          #+#    #+#             */
/*   Updated: 2022/12/01 16:31:21 by bhagenlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

char	**extend_env(char **old, char *to_add)
{
	char	*eqsign;
	char	**new;
	int	i;

	new = ft_calloc(strslen(old) + 2, sizeof(char *));
	if (new == NULL)
		return (0);
	i = -1;
	while (old[++i])
	{
		new[i] = ft_calloc(ft_strlen(old[i]) + 1, sizeof(char));
		if (new[i] == NULL)
		{
			free_strs(new);
			return (NULL);
		}
		ft_strcpy(new[i], old[i]);
	}
	new[i] = ft_calloc(ft_strlen(to_add) + 1, sizeof(char));
	if (new[i] == NULL)
	{
		free_strs(new);
		return (NULL);
	}
	if (to_add == NULL)
		return (NULL);
	eqsign = ft_strchr(to_add, '=');
	if (eqsign == NULL)
		return (NULL);
	ft_strcpy(new[i], to_add);
	free_strs(old);
	return (new);
}

char	**clone_env()
{
	extern char	**environ;
	char	**new;
	int	i;

	new = ft_calloc(strslen(environ) + 1, sizeof(char *));
	if (new == NULL)
		return (NULL);
	i = -1;
	while (environ[++i])
	{
		new[i] = ft_calloc(ft_strlen(environ[i]), sizeof(char));
		if (new[i] == NULL)
		{
			free_strs(new);
			return (NULL);
		}
		ft_strcpy(new[i], environ[i]);
	}
	return (new);
}

char	*ft_getenv(t_msh *m, char *varname)
{
	int	i;
	char	*eqsignp;
	int	eqpos;
	char	*res;

	i = -1;
	while (m->env[++i] != NULL)
	{
		eqsignp = ft_strchr(m->env[i], '=');
		eqpos = eqsignp - m->env[i];
		if (s_isneq(m->env[i], varname, eqpos))
		{
			res = ft_substr(m->env[i], eqpos + 1, ft_strlen(eqsignp + 1));
			return (res);
		}
	}
	return (NULL);
}

void	strip_env(t_msh *m, int pos, int len)
{
	free(m->env[pos]);
	ft_memmove(&m->env[pos], &m->env[pos + 1], sizeof(char *) * len);
}

int	rm_entry(t_msh *m, char *varname)
{
	int	i;
	int	to_rm;
	char	*eqsignp;
	int	eqpos;
	int	res;

	i = -1;
	res = 0;
	to_rm = 0;
	while (m->env[++i] != NULL)
	{
		eqsignp = ft_strchr(m->env[i], '=');
		eqpos = eqsignp - m->env[i];
		if (s_isneq(m->env[i], varname, eqpos))
		{
			to_rm = i;
			res = 1;
		}
	}
	if (res == 1)
	{
		strip_env(m, to_rm, i - to_rm);
	}
	return (res);
}
