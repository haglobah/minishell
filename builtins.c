/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 17:36:28 by bhagenlo          #+#    #+#             */
/*   Updated: 2023/01/11 20:53:38 by tpeters          ###   ########.fr       */
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

int	ft_cd_wrapped(t_msh *m, char **args)
{
	char	new_wd[PATH_MAX];
	char	old_wd[PATH_MAX];

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
	return (0);
}

int	ft_cd(t_msh *m, char **args)
{
	if (strslen(args) != 2)
	{
		ft_printf("cd: wrong number of arguments\n");
		return (1);
	}
	else
	{
		ft_cd_wrapped(m, args);
	}
	return (0);
}

int	ft_pwd(t_msh *m, char **args)
{
	char	*ret;

	if (strslen(args) >= 2)
	{
		ft_printf("pwd: too many arguments\n");
		return (1);
	}
	else
	{
		ret = ft_getenv(m->env, "PWD");
		ft_printf("%s\n", ret);
		if (ret)
			free(ret);
	}
	return (0);
}

int	ft_unset(t_msh *m, char **args)
{
	int	i;

	i = -1;
	while (args[++i])
	{
		rm_entry(m, args[i]);
	}
	return (0);
}
