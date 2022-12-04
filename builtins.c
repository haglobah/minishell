/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhagenlo <bhagenlo@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 17:36:28 by bhagenlo          #+#    #+#             */
/*   Updated: 2022/12/04 13:04:57 by bhagenlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

int	ft_echo(t_msh *m, char **args)
{
	int	argc;
	int	i;
	int	n_flag;

	(void)m;
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
	char	new_wd[PATH_MAX];
	char	old_wd[PATH_MAX];

	if (strslen(args) != 2)
	{
		ft_printf("cd: wrong number of arguments\n");
		return (1);
	}
	else
	{
		if (getcwd(old_wd, sizeof(old_wd)) == NULL)
		{
			ft_printf("You don't have a wd. Interesting.");
			return (1);
		}
		if (chdir(args[1]) != 0)
		{
			ft_printf("cd: No such file or directory\n");
			return (1);
		}
		else
		{
			getcwd(new_wd, sizeof(new_wd));
			ft_setenv(m, "OLDPWD", old_wd);
			ft_setenv(m, "PWD", new_wd);
		}
	}
	return (0);
}

int	ft_pwd(t_msh *m, char **args)
{
	if (strslen(args) >= 2)
	{
		ft_printf("pwd: too many arguments\n");
		return (1);
	}
	else
		ft_printf("%s\n", ft_getenv(m->env, "PWD"));
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
	{
		ft_printf("env: too many arguments\n");
		return (1);
	}
	else
		prints(*m->env);
	return (0);
}

void	free_all(t_msh *m)
{
	free_strs(*m->env);
	free(m->env);
	del_msh(m);
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
		extend_worked = extend_env(m->env, args[1]);
		if (extend_worked == false)
		{
			ft_printf("export: not valid in this context\n");
			return (1);
		}
	}
	return (0);
}
