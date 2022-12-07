/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhagenlo <bhagenlo@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 20:13:24 by bhagenlo          #+#    #+#             */
/*   Updated: 2022/12/07 11:49:08 by bhagenlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

t_ct	*mk_ct(void)
{
	t_ct	*ct;

	ct = (t_ct *)ft_calloc(1, sizeof(t_ct));
	if (!ct)
		return (NULL);
	ct->cmds = NULL;
	ct->sentences = NULL;
	ct->senc = -1;
	return (ct);
}

void	del_cmd(t_cmd *cmd)
{
	int	i;

	if (cmd)
	{
		if (cmd->argv)
			free_strs(cmd->argv);
		if (cmd->args)
		{
			i = -1;
			while (cmd->args[++i] != NULL)
			{
				ft_free(cmd->args[i]);
			}
			ft_free(cmd->args);
		}
		if (cmd->in)
			ft_free(cmd->in);
		if (cmd->here)
			ft_free(cmd->here);
		if (cmd->out)
			ft_free(cmd->out);
		ft_free(cmd);
	}
}

void	del_ct(t_ct *ct)
{
	int	i;

	if (ct != NULL)
	{
		i = -1;
		while (++i < ct->senc)
		{
			if (ct->cmds != NULL)
				del_cmd(ct->cmds[i]);
			if (ct->sentences != NULL)
				free_strs(ct->sentences[i]);
		}
		if (ct->cmds != NULL)
			ft_free(ct->cmds);
		if (ct->sentences != NULL)
			ft_free(ct->sentences);
		ft_free(ct);
	}
}

//malloc minishell-struct
t_msh	*mk_msh(char **toks, char ***env, char *t, int *rv)
{
	t_msh	*msh;

	msh = (t_msh *)ft_calloc(1, sizeof(t_msh));
	if (!msh)
		return (NULL);
	msh->t = t;
	msh->toks = toks;
	msh->ct = mk_ct();
	msh->env = env;
	msh->rv = rv;
	return (msh);
}

void	del_msh(t_msh *m)
{
	if (m)
	{
		if (m->t)
			ft_free(m->t);
		if (m->toks)
			free_strs(m->toks);
		if (m->ct)
			del_ct(m->ct);
		ft_free(m);
	}
}

void	free_all(t_msh *m)
{
	free_strs(*m->env);
	ft_free(m->env);
	del_msh(m);
}
