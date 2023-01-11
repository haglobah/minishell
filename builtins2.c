/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 20:53:27 by tpeters           #+#    #+#             */
/*   Updated: 2023/01/11 20:53:48 by tpeters          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

int	ft_env(t_msh *m, char **args)
{
	if (strslen(args) >= 2)
	{
		ft_printf("env: too many arguments\n");
		return (1);
	}
	else
		prints(*m->env);
	return (0);
}

int	ft_exit(t_msh *m, char **args)
{
	int	code;

	code = 0;
	if (strslen(args) >= 3)
	{
		ft_printf("exit: too many arguments\n");
		return (1);
	}
	if (strslen(args) == 2)
	{
		if (!ft_parse_int(args[1], &code))
		{
			ft_printf("exit: non-numerical argument: %s\n", args[1]);
			return (1);
		}
	}
	ft_printf("code: %i\n", code);
	free_all(m);
	exit(code);
}

int	ft_export(t_msh *m, char **args)
{
	bool	extend_worked;

	extend_worked = true;
	if (strslen(args) != 2)
	{
		ft_printf("export: wrong number of arguments\n");
		return (1);
	}
	else
	{
		extend_worked = extend_env(m, args[1]);
		if (extend_worked == false)
		{
			ft_printf("export: not valid in this context\n");
			return (1);
		}
	}
	return (0);
}
