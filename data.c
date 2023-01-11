/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 20:13:24 by bhagenlo          #+#    #+#             */
/*   Updated: 2023/01/11 20:54:43 by tpeters          ###   ########.fr       */
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
	if (cmd)
	{
		if (cmd->argv)
			free_strs(cmd->argv);
		if (cmd->args)
			free_strs(cmd->args);
		if (cmd->in)
			ft_free((void **)&cmd->in);
		if (cmd->here)
			ft_free((void **)&cmd->here);
		if (cmd->out)
			ft_free((void **)&cmd->out);
		ft_free((void **)&cmd);
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
			ft_free((void **)&ct->cmds);
		if (ct->sentences != NULL)
			ft_free((void **)&ct->sentences);
		ft_free((void **)&ct);
	}
}

//malloc minishell-struct
t_msh	*mk_msh(char **toks, char ***env, t_in *in, int *rv)
{
	t_msh	*msh;

	msh = (t_msh *)ft_calloc(1, sizeof(t_msh));
	if (!msh)
		return (NULL);
	msh->in = in;
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
		if (m->in->here_did_realloc)
			ft_free((void **)&m->in->t);
		free_strspl(m->toks, "toks");
		if (m->ct)
			del_ct(m->ct);
		ft_free((void **)&m);
	}
}
