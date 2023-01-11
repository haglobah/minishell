/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 16:31:21 by bhagenlo          #+#    #+#             */
/*   Updated: 2023/01/11 20:56:18 by tpeters          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

int	copy_env(char **new, char **old)
{
	int	i;

	i = -1;
	while (old[++i] != NULL)
	{
		new[i] = ft_calloc(ft_strlen(old[i]) + 1, sizeof(char));
		if (new[i] == NULL)
		{
			free_strs(new);
			return (-1);
		}
		ft_strcpy(new[i], old[i]);
	}
	return (i);
}

bool	add_entry(char ***env, char *to_add)
{
	char	*eqsign;
	char	**new;
	int		envlen;

	if (to_add == NULL)
		return (false);
	new = ft_calloc(strslen(*env) + 2, sizeof(char *));
	if (new == NULL)
		return (false);
	envlen = copy_env(new, *env);
	if (envlen == -1)
		return (false);
	new[envlen] = ft_calloc(ft_strlen(to_add) + 1, sizeof(char));
	if (new[envlen] == NULL)
		return (free_strsb(new));
	eqsign = ft_strchr(to_add, '=');
	if (eqsign == NULL)
		return (free_strsb(new));
	ft_strcpy(new[envlen], to_add);
	free_strs((*env));
	*env = new;
	return (true);
}

bool	extend_env(t_msh *m, char *to_add)
{
	char	*var;
	char	*eqsignp;
	char	*varname;
	int		eqpos;

	eqsignp = ft_strchr(to_add, '=');
	eqpos = eqsignp - to_add;
	varname = ft_substr(to_add, 0, eqpos);
	if (!varname)
		return (NULL);
	var = ft_getenv(m->env, varname);
	if (var)
	{
		rm_entry(m, varname);
		free(var);
	}
	free(varname);
	return (add_entry(m->env, to_add));
}

char	**clone_env(void)
{
	extern char	**environ;
	char		**new;
	int			i;

	new = ft_calloc(strslen(environ) + 1, sizeof(char *));
	if (new == NULL)
		return (NULL);
	i = -1;
	while (environ[++i])
	{
		new[i] = ft_calloc(ft_strlen(environ[i]) + 1, sizeof(char));
		if (new[i] == NULL)
		{
			free_strs(new);
			return (NULL);
		}
		ft_strcpy(new[i], environ[i]);
	}
	return (new);
}

bool	ft_setenv(t_msh *m, char *name, char *value)
{
	int		i;
	char	*eqsignp;
	int		eqpos;
	char	*res;
	char	**env;

	i = -1;
	env = *m->env;
	while (env[++i] != NULL)
	{
		eqsignp = ft_strchr(env[i], '=');
		eqpos = eqsignp - env[i];
		if (s_isneq(env[i], name, eqpos))
		{
			res = ft_calloc(sizeof(char), (eqpos + 1) + ft_strlen(value) + 1);
			ft_strlcpy(res, env[i], eqpos + 2);
			ft_strlcpy(&res[eqpos + 1], value, ft_strlen(value) + 1);
			ft_free((void **)&env[i]);
			env[i] = res;
		}
	}
	return (NULL);
}
