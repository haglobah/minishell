/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 20:55:53 by tpeters           #+#    #+#             */
/*   Updated: 2023/01/11 20:56:27 by tpeters          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

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
	ft_free((void **)&(*m->env)[pos]);
	ft_memmove((*m->env) + pos, (*m->env) + pos + 1, sizeof(char *) * len);
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
