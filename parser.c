/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhagenlo <bhagenlo@student.42heil...>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 17:00:48 by bhagenlo          #+#    #+#             */
/*   Updated: 2022/09/21 17:00:48 by bhagenlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

static int	s_isneq(char *s1, char *s2, int n)
{
	return (!ft_strncmp(s1, s2, n));
}

static int	s_iseq(char *s1, char *s2)
{
	return (!ft_strcmp(s1, s2));
}

static int	consists_of_only(char *token, char *chars)
{
	while (token)
	{
		if (!char_in_set(*token, chars))
			return (0);
		token++;
	}
	return (1);
}

int	is_empty(t_list *t)
{
	return (1); //Stimmt das sicher?
}

int	word(t_list *t)
{
	if (!(consists_of_only(t->content, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789")))
		return (0);

	return (1);
}

int	ass_word(t_list *t)
{
	if (!(s_isneq(t->content, "$", 1) && word(t)))
		return (0);

	return (1);
}

int	here_end(t_list *t)
{
	if (!(word(t)))
		return (0);

	return (1);
}

int	is_nl(t_list *t)
{
	return(!s_isneq(t->content, "\n", 1));
}

int	nl_list(t_list *t)
{
	if (!(is_nl(t) || nl_list(t) && is_nl(t)))
		return (0);
	
	return (1);
}

int	linebr(t_list *t)
{
	if (!(nl_list(t) || is_empty(t)))
		return (0);
	
	return (1);
}

//does this work in the tokenizer? Looks like a challenge to me.
int	io_here(t_list *t)
{
	if (!(s_iseq(t->content, "<<") && here_end(t)))
		return (0);

	return (1);
}

int	filename(t_list *t)
{
	if (!(word(t)))
		return (0);

	return (1);
}

int	io_file(t_list *t)
{
	if (!(s_iseq(t->content, "<") && filename(t)
		  || s_iseq(t->content, ">") && filename(t)
		  || s_iseq(t->content, ">>") && filename(t)))
		return (0);

	return (1);
}

int	io_redir(t_list *t)
{
	if (!(io_file(t) || io_here(t)))
		return (0);

	return (1);
}

// We don't call redirect_list anywhere...
int	redir_list(t_list *t)
{
	if (!(redir_list(t) && io_redir(t)
		  || io_redir(t)))
		return (0);

	return (1);
}

int	c_word(t_list *t)
{
	if (!(word(t)))
		return (0);

	return (1);
}

int	c_name(t_list *t)
{
	if (!(word(t)))
		return (0);

	return (1);
}

int	c_suff(t_list *t)
{
	if ( !(io_redir(t)
		   || c_suff(t) && io_redir(t)
		   || word(t)
		   || c_suff(t) && word(t)))
		return (0);

	return (1);
}

int	c_pref(t_list *t)
{
	if ( !(io_redir(t)
		   || c_pref(t) && io_redir(t)
		   || ass_word(t)
		   || c_pref(t) && ass_word(t)))
		return (0);

	return (1);
}

int	cmd(t_list *t)
{
	if ( !(c_pref(t) && c_word(t) && c_suff(t)
		|| c_pref(t) && c_word(t)
		|| c_pref(t)
		|| c_name(t) && c_suff(t)
		|| c_name(t)))
		return (0);

	return (1);
}

int	pip(t_list *t)
{
	if (!(s_iseq(t->content, "|")))
		return (0);

	return (1);
}

int	pipe_seq(t_list *t)
{
	if (!(cmd(t) || pipe_seq(t) && pip(t) && linebr(t) && cmd(t)))
		return (0);

	return (1);
}

int	cpl_cmds(t_list *t)
{
	if (!(cpl_cmds(t) && nl_list(t) && pipe_seq(t) || pipe_seq(t)))
		return (0);
	return (1);
}

int	program(t_list *t)
{
	if (!(linebr(t) || linebr(t) && cpl_cmds(t) && linebr(t)))
		return (0);

	return (1);
}

int	parse(t_list *toks)
{
	if (!program(toks))
		return (0);

	return (1);
}
