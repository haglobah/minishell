/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhagenlo <bhagenlo@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 20:13:24 by bhagenlo          #+#    #+#             */
/*   Updated: 2022/12/04 14:19:46 by bhagenlo         ###   ########.fr       */
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
	free_strs(cmd->argv);
	// free_strs(cmd->args); TODO: Why hasn't this been allocated?
	free(cmd->in);
	free(cmd->out);
	free(cmd);
}

void	del_ct(t_ct *ct)
{
	int	i;

	i = -1;
	while (++i < ct->senc)
	{
		del_cmd(ct->cmds[i]);
		free_strs(ct->sentences[i]);
	}
	free(ct->cmds);
	free(ct->sentences);
	free(ct);
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
			free(m->t);
		if (m->toks)
			free_strs(m->toks);
		if (m->ct)
			del_ct(m->ct);
		free(m);
	}
}

void	free_all(t_msh *m)
{
	free_strs(*m->env);
	free(m->env);
	del_msh(m);
}
