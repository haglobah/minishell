/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhagenlo <bhagenlo@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/03 19:40:37 by bhagenlo          #+#    #+#             */
/*   Updated: 2022/12/03 19:41:04 by bhagenlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

char	*search_PATH(t_msh *m, char *exec_name)
{
	char	*exec_path;
	char	*tmppath;
	char	*slash_name;
	char	**paths;
	int		i;

	exec_path = NULL;
	paths = ft_split(ft_getenv(m, "PATH"), ':');
	if (paths == NULL)
		return (NULL);
	printns(paths);
	i = -1;
	while (paths[++i] != NULL)
	{
		slash_name = ft_strjoin("/", exec_name);
		tmppath = ft_strjoin(paths[i], slash_name);
		ft_printf("'%s'\n", tmppath);
		if (access(tmppath, F_OK) == 0)
		{
			exec_path = tmppath;
			break ;
		}
		free(tmppath);
	}
	if (exec_path == NULL)
	{
		ft_printf("%s: command not found\n", exec_name);
	}
	free_strs(paths);
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
		path = search_PATH(m, args[0]);
	}
	ft_printf("path: %s\n", path);
	return (path);
}

t_execve	*mk_execve(t_msh *m, t_cmd *cmd)
{
	t_execve	*ev;

	ev = ft_calloc(sizeof(t_execve), 1);
	if (ev == NULL)
		return (NULL);
	ev->args = cmd->args;
	ev->pathname = find_path(m, ev->args);
	ev->env = clone_env();
	return (ev);
}

void	del_execve(t_execve *ev)
{
	free_strs(ev->args);
	free(ev->pathname);
	free_strs(ev->env);
	free(ev);
}