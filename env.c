/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhagenlo <bhagenlo@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 16:31:21 by bhagenlo          #+#    #+#             */
/*   Updated: 2022/12/04 18:27:19 by bhagenlo         ###   ########.fr       */
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

bool	extend_env(char ***env, char *to_add)
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
			free(env[i]);
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
	int		to_rm;
	int		res;
	t_env	e;

	e = (t_env){.i = -1, .eqsignp = NULL, .eqpos = 0, .env = *m->env};
	res = 0;
	to_rm = 0;
	while (e.env[++e.i] != NULL)
	{
		e.eqsignp = ft_strchr(e.env[e.i], '=');
		e.eqpos = e.eqsignp - e.env[e.i];
		if (s_isneq(e.env[e.i], varname, e.eqpos))
		{
			to_rm = e.i;
			res = 1;
		}
	}
	if (res == 1)
	{
		strip_env(m, to_rm, e.i - to_rm);
	}
	return (res);
}
