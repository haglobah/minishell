/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhagenlo <bhagenlo@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 17:36:28 by bhagenlo          #+#    #+#             */
/*   Updated: 2022/12/03 16:16:49 by bhagenlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

int	ft_echo(t_msh *m, char **args)
{
	int	argc;
	int	i;
	int	n_flag;

	n_flag = 0;
	i = 1;
	argc = strslen(args);
	if (s_iseq(args[1], "-n"))
	{
		i++;
		n_flag = 1;
	}
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (!n_flag || (n_flag && i < argc - 1))
			ft_printf("\n");
		i++;
	}
	return (0);
}
int	ft_cd(t_msh *m, char **args)
{
	if (strslen(args) != 2)
		ft_printf("cd: wrong number of arguments\n");
	else
	{
		if (chdir(args[1]) != 0)
		{
			ft_printf("cd: No such file or directory\n");
		}
		else
		{
			char *argums[] = {"env", NULL};
			ft_env(m, argums);
			
		}
	}
	return (0);
}
int	ft_pwd(t_msh *m, char **args)
{
	if (strslen(args) >= 2)
		ft_printf("pwd: too many arguments\n");
	else
		ft_printf("%s\n", ft_getenv(m, "PWD"));
	return (0);
}
int	ft_unset(t_msh *m, char **args)
{
	int	i;

	i = -1;
	while (args[++i])
	{
		rm_entry(m, args[i]);
		// or rather a sane version of it? With _feedback_!?
	}
	return (0);
}
int	ft_env(t_msh *m, char **args)
{
	if (strslen(args) >= 2)
		ft_printf("env: too many arguments\n");
	else
		prints(*m->env);
	return (0);
}
int	ft_exit(t_msh *m, char **args)
{
	//arg handling
	ft_printf("Should exit the shell.\n");
	return (0);
}

int	ft_export(t_msh *m, char **args)
{
	bool	extend_worked;

	extend_worked = true;
	if (strslen(args) != 2)
		ft_printf("export: wrong number of arguments\n");
	else
	{
		// prints(m->env);
		//some bug in it on double export.
		extend_worked = extend_env(m->env, args[1]);
		if (extend_worked == false)
		{
			ft_printf("export: not valid in this context\n");
			return (0);
		}
		// prints(m->env);
	}
	return (1);
}
