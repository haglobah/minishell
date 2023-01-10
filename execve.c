/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/03 19:40:37 by bhagenlo          #+#    #+#             */
/*   Updated: 2023/01/09 20:00:41 by tpeters          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

bool	construct_exec_path(char **paths, char *exec_name, char **exec_path)
{
	char	*tmppath;
	char	*slash_name;
	int		i;

	i = -1;
	while (paths[++i] != NULL)
	{
		slash_name = ft_strjoin("/", exec_name);
		if (slash_name == NULL)
			return (NULL);
		tmppath = ft_strjoin(paths[i], slash_name);
		if (tmppath == NULL)
			return (ft_free((void **)&slash_name), NULL);
		ft_free((void **)&slash_name);
		if (access(tmppath, F_OK) == 0)
		{
			*exec_path = tmppath;
			return (true);
		}
		ft_free((void **)&tmppath);
	}
	return (false);
}

char	*search__path_(t_msh *m, char *exec_name)
{
	char	*exec_path;
	char	**paths;
	char	*pathstr;

	exec_path = NULL;
	pathstr = ft_getenv(m->env, "PATH");
	if (!pathstr)
		return (NULL);
	paths = ft_split(pathstr, ':');
	if (paths == NULL)
		return (ft_free((void **)&pathstr), NULL);
	construct_exec_path(paths, exec_name, &exec_path);
	free_strs(paths);
	ft_free((void **)&pathstr);
	return (exec_path);
}

char	*find_path(t_msh *m, char **args)
{
	char	*path;

	if (s_isneq(args[0], "/", 1)
		|| s_isneq(args[0], "./", 2)
		|| s_isneq(args[0], "../", 3))
	{
		path = ft_calloc(sizeof(char), ft_strlen(args[0]) + 1);
		if (path == NULL)
			return (NULL);
		ft_strcpy(path, args[0]);
	}
	else
	{
		path = search__path_(m, args[0]);
	}
	return (path);
}

t_execve	*mk_execve(t_msh *m, t_cmd *cmd)
{
	t_execve	*ev;

	ev = ft_calloc(sizeof(t_execve), 1);
	if (ev == NULL)
		return (NULL);
	ev->args = cmd->args;
	if (ev->args == NULL)
	{
		ft_free((void **)&ev);
		return (NULL);
	}
	ev->pathname = find_path(m, ev->args);
	if (!ev->pathname)
	{
		ft_printf("%s: command not found\n", ev->args[0]);
		ft_free((void **)&ev);
		return (NULL);
	}
	return (ev);
}

// CAUTION: We don't want to ever free ev->args. Otherwise we free twice.
void	del_execve(t_execve *ev)
{
	if (ev)
	{
		if (ev->pathname)
			ft_free((void **)&ev->pathname);
		ft_free((void **)&ev);
	}
}
