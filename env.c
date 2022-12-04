/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhagenlo <bhagenlo@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 16:31:21 by bhagenlo          #+#    #+#             */
/*   Updated: 2022/12/04 17:55:42 by bhagenlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

bool	extend_env(char ***env, char *to_add)
{
	char	*eqsign;
	char	**new;
	int		i;

	if (to_add == NULL)
		return (false);
	new = ft_calloc(strslen(*env) + 2, sizeof(char *));
	if (new == NULL)
		return (false);
	i = -1;
	while ((*env)[++i])
	{
		new[i] = ft_calloc(ft_strlen((*env)[i]) + 1, sizeof(char));
		if (new[i] == NULL)
		{
			free_strs(new);
			return (false);
		}
		ft_strcpy(new[i], (*env)[i]);
	}
	new[i] = ft_calloc(ft_strlen(to_add) + 1, sizeof(char));
	if (new[i] == NULL)
	{
		free_strs(new);
		return (false);
	}
	eqsign = ft_strchr(to_add, '=');
	if (eqsign == NULL)
	{
		free_strs(new);
		return (false);
	}
	ft_strcpy(new[i], to_add);
	free_strs((*env));
	*env = new;
	return (true);
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
			env[i] = res;
		}
	}
	return (NULL);
}

char	*ft_getenv(char ***msh_env, char *varname)
{
	int		i;
	char	*eqsignp;
	int		eqpos;
	char	*res;
	char	**env;

	i = -1;
	env = *msh_env;
	while (env[++i] != NULL)
	{
		eqsignp = ft_strchr(env[i], '=');
		eqpos = eqsignp - env[i];
		if (s_isneq(env[i], varname, eqpos))
		{
			res = ft_substr(env[i], eqpos + 1, ft_strlen(eqsignp + 1));
			return (res);
		}
	}
	return (NULL);
}

void	strip_env(t_msh *m, int pos, int len)
{
	free((*m->env)[pos]);
	ft_memmove((*m->env)[pos], (*m->env)[pos + 1], sizeof(char *) * len);
}

int	rm_entry(t_msh *m, char *varname)
{
	int		i;
	int		to_rm;
	char	*eqsignp;
	char	**env;
	int		eqpos;
	int		res;

	i = -1;
	res = 0;
	to_rm = 0;
	env = *m->env;
	while (env[++i] != NULL)
	{
		eqsignp = ft_strchr(env[i], '=');
		eqpos = eqsignp - env[i];
		if (s_isneq(env[i], varname, eqpos))
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
