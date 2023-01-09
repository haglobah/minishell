/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 11:26:40 by bhagenlo          #+#    #+#             */
/*   Updated: 2023/01/09 22:36:40 by tpeters          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

char	*get_var(char **sp, int dollar)
{
	int	i;

	i = 0;
	while (char_in_set((*sp)[++i + dollar],
		"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"))
		;
	if (i == 1)
		return (NULL);
	return (ft_substr(*sp, dollar + 1, i));
}

void	replace_string_at(char **sp, int dollar, char *content, int vlen)
{
	t_rep	t;

	t = (t_rep){-1, -1, 0, 0, 0, NULL};
	t.newlen = ft_strlen(*sp) + ft_strlen(content) - (vlen + 1);
	t.new_arg = ft_calloc(sizeof(char), t.newlen + 1);
	while (++t.i < dollar)
	{
		t.new_arg[t.i] = (*sp)[t.i];
	}
	while (++t.j < (int)ft_strlen(content))
	{
		t.new_arg[t.i + t.j] = content[t.j];
	}
	t.k = t.i + t.j;
	t.z = t.i + vlen + 1;
	while ((*sp)[t.z])
	{
		t.new_arg[t.k] = (*sp)[t.z];
		t.k++;
		t.z++;
	}
	ft_free((void **)&(*sp));
	*sp = t.new_arg;
}

void	find_replace(t_msh *m, char **sp, int dollar)
{
	int		vlen;
	char	*content;
	char	*varname;

	vlen = 0;
	content = NULL;
	varname = get_var(sp, dollar);
	if (varname != NULL)
	{
		vlen = ft_strlen(varname);
		content = ft_getenv(m->env, varname);
		if (content != NULL)
		{
			replace_string_at(sp, dollar, content, vlen);
			ft_free((void **)&content);
		}
		else
		{
			replace_string_at(sp, dollar, "", vlen);
		}
		ft_free((void **)&varname);
	}
}

void	insert_rv(t_msh *m, char **sp, int dollar)
{
	char	*str;

	str = ft_itoa(*(m->rv));
	replace_string_at(sp, dollar, str, 1);
}

int	rm_quotes(char **sp, int here_quoted)
{
	int		is_quoted;
	char	*tmp;

	if (here_quoted)
		return (1);
	is_quoted = 0;
	if (!char_in_set((*sp)[0], "'\""))
		return (is_quoted);
	if ((*sp)[0] == '\'')
		is_quoted = 1;
	tmp = *sp;
	*sp = ft_substr(*sp, 1, ft_strlen(*sp) - 2);
	ft_free((void **)&tmp);
	return (is_quoted);
}

void	swap_in_vars(t_msh *m, char **sp, int here_quoted)
{
	int	i;

	if (*sp == NULL)
		return ;
	if (rm_quotes(sp, here_quoted))
		return ;
	i = -1;
	while ((*sp)[++i])
	{
		if ((*sp)[i] == '$')
		{
			if ((*sp)[i + 1] == '?')
				insert_rv(m, sp, i);
			else
				find_replace(m, sp, i);
		}
	}
}

char	**con_args_wrapped(t_cmd *cmd, char **args)
{
	int		i;
	int		j;

	i = -1;
	j = 0;
	while (++i < cmd->argc)
	{
		if (s_iseq(cmd->argv[i], ""))
			continue ;
		args[j] = ft_calloc(sizeof(char), ft_strlen(cmd->argv[i]) + 1);
		if (args[j] == NULL)
			return (NULL);
		ft_strcpy(args[j], cmd->argv[i]);
		j++;
	}
	return (args);
}

char	**cons_args(t_cmd *cmd)
{
	int		i;
	int		empty_count;
	char	**args;

	empty_count = 0;
	i = -1;
	while (++i < cmd->argc)
	{
		if (s_iseq(cmd->argv[i], ""))
			empty_count += 1;
	}
	if (i == 0)
		return (NULL);
	args = ft_calloc(sizeof(char *), (i - empty_count + 1));
	if (args == NULL)
		return (NULL);
	return (con_args_wrapped(cmd, args));
}

void	loop_through_cmds(t_msh *m)
{
	int		i;
	int		j;
	t_cmd	**cmds;

	cmds = m->ct->cmds;
	i = -1;
	while (cmds[++i] != NULL)
	{
		j = -1;
		while (++j < cmds[i]->argc)
		{
			swap_in_vars(m, &cmds[i]->argv[j], cmds[i]->here_quoted);
		}
		swap_in_vars(m, &cmds[i]->in, cmds[i]->here_quoted);
		swap_in_vars(m, &cmds[i]->out, cmds[i]->here_quoted);
		cmds[i]->args = cons_args(cmds[i]);
	}
}

int	evaluate(t_msh *m)
{
	loop_through_cmds(m);
	return (0);
}
