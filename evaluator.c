/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhagenlo <bhagenlo@student.42heil...>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 11:26:40 by bhagenlo          #+#    #+#             */
/*   Updated: 2022/11/22 11:26:40 by bhagenlo         ###   ########.fr       */
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

void	find_replace(t_msh *m, char **sp, int dollar)
{
	int	i;
	int	j;
	int	k;
	int	z;
	int	newlen;
	int	clen;
	int	vlen;
	char	*new_arg;
	char	*content = NULL;

	clen = 0;
	vlen = 0;
	char *varname = get_var(sp, dollar);
	if (varname != NULL)
	{
		vlen = ft_strlen(varname);
		content = ft_getenv(m, varname);
		if (content != NULL)
		{
			clen = ft_strlen(content);
		}
	}
	newlen = ft_strlen(*sp) + clen - (vlen + 1);
	new_arg = ft_calloc(sizeof(char), newlen + 1);
	i = -1;
	while (++i < dollar)
	{
		new_arg[i] = (*sp)[i];
	}
	j = -1;
	while (++j < clen)
	{
		new_arg[i + j] = content[j];
	}
	k = i + j;
	z = i + vlen + 1;
	while ((*sp)[z])
	{
		new_arg[k] = (*sp)[z];
		k++;
		z++;
	}
	free(varname);
	//free(content) -> better not – "You must not modify this string".
	free(*sp);
	*sp = new_arg;
}

void	insert_error(char **sp, int dollar)
{
	(void)sp;
	(void)dollar;
	ft_printf("\n  Should store the val of the last execution.\n  Not yet implemented.\n");
}

int	rm_quotes(char **sp, int here_quoted)
{
	int	is_quoted;
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
	free(tmp);
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
				insert_error(sp, i);
			else
				find_replace(m, sp, i);
		}
	}
}

char **cons_args(t_cmd *cmd)
{
	int	i;
	int	j;
	int	empty_count;
	char	**args;

	empty_count = 0;
	i = -1;
	while (++i < cmd->argc)
	{
		if (s_iseq(cmd->argv[i], ""))
			empty_count += 1;
	}
	args = ft_calloc(sizeof(char *), (i - empty_count + 1));
	if (args == NULL)
		return (NULL);
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

void	loop_through_cmds(t_msh *m)
{
	int	i;
	int	j;
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
		ft_printf("\n");
		printcmd(m->ct->cmds[i]);
	}
}

int	evaluate(t_msh *m)
{
	loop_through_cmds(m);
	return (0);
}
