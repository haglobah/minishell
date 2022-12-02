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

int	export(/* char ***env */)
{
	char	**env;

	env = clone_env();
	if (env == NULL)
		return (0);
	prints(env);
	env = extend_env(env, "BLAH=42");
	prints(env);
	free_strs(env);
	return (1);
}

int	unset(/* char ***env */)
{
	
}
