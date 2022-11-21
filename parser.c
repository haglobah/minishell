/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhagenlo <bhagenlo@student.42heil...>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 17:00:48 by bhagenlo          #+#    #+#             */
/*   Updated: 2022/09/25 19:15:17 by bhagenlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"
void	*ft_realloc(void *ptr, size_t size)
{
	void	*new_ptr;

	if (ptr == NULL)
		return (malloc(size));
	if (!size)
		return (ptr);
	new_ptr = malloc(size);
	ft_memcpy(new_ptr, ptr, size);
	free(ptr);
	return (new_ptr);
}

int	s_isneq(char *s1, char *s2, int n)
{
	if (!s1)
		ft_printf("There is no token.\n");
	return (!ft_strncmp(s1, s2, n));
}

int	s_iseq(char *s1, char *s2)
{
	return (!ft_strcmp(s1, s2));
}

int	consists_of_only(char *token, char *chars)
{
	while (*token)
	{
		if (!char_in_set(*token, chars))
		{
			ft_printf("Not in set: %c\n", *token);
			return (0);
		}
		token++;
	}
	return (1);
}

static int	s_in_s(char *s, char **slist)
{
	int	i;

	i = 0;
	while (slist[i])
	{
		if(s_iseq(s, slist[i]))
			return (1);
		i++;
	}
	return (0);
}

int	is_empty(t_list *t)
{
	return (1); //Stimmt das sicher?
}

int	word(t_list *t)
{
	if (!(consists_of_only(t->content, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789")))
		return (0);
	//ft_printf("Is a word: %s", t->content);
	return (1);
}

int	ass_word(t_list *t)
{
	if (s_isneq(t->content, "$", 1))
	{
		t->content++;
		if (word(t))
		{
			return (1);
		}
	}
	return (0);
}

int	here_end(t_list *t)
{
	if (!(word(t)))
		return (0);
	ft_printf("Here_end");
	return (1);
}

int	is_nl(t_list *t)
{
	return(s_isneq(t->content, "\n", 1));
}

int	nl_list(t_list *t)
{
	while (t->content)
	{
		ft_printf("%c", *(char *)t->content);
		if (*(char *)t->content == '\n')
		{
			ft_printf("nl\n");
			t->content++;
		}
		else
		{
			return (0);
		}
	}
	return (1);
}

int	linebr(t_list *t)
{
	if (!(nl_list(t) || is_empty(t)))
		return (0);
	ft_printf("linebr");
	return (1);
}

//does this work in the tokenizer? Looks like a challenge to me.
int	io_here(t_list *t)
{
	if (!(s_iseq(t->content, "<<") && here_end(t)))
		return (0);
	ft_printf("HERE String");
	return (1);
}

int	filename(t_list *t)
{
	if (!(word(t)))
		return (0);
	ft_printf("filename");
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
	ft_printf("redirection");
	return (1);
}

int	c_word(t_list *t)
{
	if (!(word(t)))
		return (0);
	ft_printf("command word");
	return (1);
}

int	c_name(t_list *t)
{
	if (!(word(t)))
		return (0);
	ft_printf("command Name");
	return (1);
}

int	c_suff(t_list *t)
{
	if ( !(io_redir(t)
		   || c_suff(t) && io_redir(t)
		   || word(t)
		   || c_suff(t) && word(t)))
		return (0);
	ft_printf("c_suffix");
	return (1);
}

int	c_pref(t_list *t)
{
	if ( !(io_redir(t)
		   || c_pref(t) && io_redir(t)
		   || ass_word(t)
		   || c_pref(t) && ass_word(t)))
		return (0);
	ft_printf("command Prefix");
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
	ft_printf("command");
	return (1);
}

int	pip(t_list *t)
{
	if (!(s_iseq(t->content, "|")))
		return (0);
	ft_printf("Pipe");
	return (1);
}

int	pipe_seq(t_list *t)
{
	if (!(cmd(t) || pipe_seq(t) && pip(t) && linebr(t) && cmd(t)))
		return (0);
	ft_printf("pipe_sequence");
	return (1);
}

int	cpl_cmds(t_list *t)
{
	if (!(cpl_cmds(t) && nl_list(t) && pipe_seq(t) || pipe_seq(t)))
		return (0);
	ft_printf("complete_commands");
	return (1);
}

int	program(t_list *t)
{
	if (!(linebr(t) || linebr(t) && cpl_cmds(t) && linebr(t)))
		return (0);
	ft_printf("Program");
	return (1);
}

int	parse_real_list(t_list *toks)
{
	if (!program(toks))
		return (0);
	ft_printf("Parse");
	return (1);
}

int	parse_list(t_list *t)
{
	if (!t)
	{
		//ft_printf("No list. \n");
		return (1);
	}
	if (!t->content)
		ft_printf("No content.\n ");
	ft_printf("t->content: %s\n", t->content);
	if (pipe_seq(t))
		return (1);
	return (0);
}

int	throw_error(char *token)
{
	ft_printf("bash: syntax error near unexpected token '%s'\n", token);
}


//Beware: c = 1! (pipecount one to high)
int	count_pipes(char **toks, int *places)
{
	int	c;
	int	i;

	i = 0;
	c = 1;
	while (toks[i])
	{
		if (s_iseq(toks[i], "|"))
		{
			places[c] = i + 1;
			free(toks[i]);
			toks[i] = NULL;
			c++;
		}
		i++;
	}
	places[c] = i;
	return (c);
}

int	put_split_to_table(t_msh *m, int pi, int *pipe_places)
{
	int	tok_count;
	int	sen_i;
	int	tok_place;

	sen_i = 0;
	tok_count = pipe_places[pi + 1] - pipe_places[pi];
	m->ct->sentences[pi] = (char **)ft_calloc(tok_count + 1, sizeof(char *));
	while (sen_i < tok_count)
	{
		tok_place = pipe_places[pi] + sen_i;
		if (m->toks[tok_place])
		{
			m->ct->sentences[pi][sen_i] = ft_strdup(m->toks[tok_place]);
			//ft_printf("%s, pi: %d \n", m->ct->sentences[pi][sen_i], pi);
		}
		sen_i++;
	}
}

int	print_sens(t_msh *m)
{
	int	i;
	int	j;

	i = 0;
	ft_printf(" ");
	while (m->ct->sentences[i])
	{
		j = 0;
		while (m->ct->sentences[i][j])
		{
			ft_printf("  %s", m->ct->sentences[i][j]);
			j++;
		}
		ft_printf("\n|");
		i++;
	}
	ft_printf("\n");
	return (1);
}

int	split_by_pipes(t_msh *m)
{
	int	i;
	int	pipecount;
	int	*pipe_places;

	pipe_places = (int *)ft_calloc(NUM_PIPES + 1, sizeof(int));
	if (!pipe_places)
		return (-2);
	pipecount = count_pipes(m->toks, pipe_places);
//	ft_printf("# of pipes: %i\n", pipecount);
	m->ct->sentences = (char ***)ft_calloc(pipecount + 1, sizeof(char **));
	m->ct->senc = pipecount;
//	ft_printf("%i", m->ct->senc);
	if (!m->ct->sentences)
		return (-3);
	i = 0;
	while (i < pipecount)
	{
		//one precommand (only split by pipe per table row.
		put_split_to_table(m, i, pipe_places);
		i++;
	}
//	print_sens(m);
	free(pipe_places);
	return (pipecount);
}

int	pipe_checkup(t_msh *m)
{
	int	i;

	i = 0;
	while (m->ct->sentences[i])
	{
		if (!m->ct->sentences[i][0])
		{
			throw_error("|");
			return (0);
		}
		i++;
	}
}

int	check_redirs(t_msh *m, char **sen)
{
	int	i;
	int	rc;
	char	*redirs[] = {"<", ">", "<<", ">>", NULL};

	i = 0;
	rc = 0;
	while (sen[i])
	{
		if (s_in_s(sen[i], redirs))
		{
			if (!sen[i + 1])
			{
				throw_error(sen[i]);
				return (-1);
			}
			if (s_in_s(sen[i + 1], redirs))
			{
				throw_error(sen[i + 1]);
				return (-1);
			}
			rc += 2;
		}
		i++;
	}
	return (i - rc);
}

char	*parse_here(t_cmd *cmd, char *here)
{
	int	is_quoted;
	int	i;
	int	j;

	j = ft_strlen(here) - 2;
	while (here[j] != '\n')
	{
		j--;
	}
	i = 0;
	//is_quoted
	if (char_in_set(here[i], "'\""))
	{
		//del_delim
		i++;
		while (!char_in_set(here[i], "'\""))
			i++;
		i++;
		cmd->here_quoted = 1;
	}
	else
	{
		i += ft_strlen(here) - 2 - j;
	}
	return (ft_substr(here, i, j - i));
}

int	compute_io(t_msh *m, char **sen, t_cmd *cmd)
{
	int	i;
	int	j;

	cmd->argc = check_redirs(m, sen);
	if(cmd->argc == -1)
		return (-1);
	cmd->appp = 0;
	cmd->argv = (char **)ft_calloc(cmd->argc + 1, sizeof(char *));
	i = 0;
	j = 0;
	while (sen[i])
	{
		if (s_iseq(sen[i], "<"))
		{
			cmd->in = ft_strdup(sen[i + 1]);
			i++;
		}
		else if (s_iseq(sen[i], ">"))
		{
			cmd->out = ft_strdup(sen[i + 1]);
			i++;
		}
		else if (s_iseq(sen[i], "<<"))
		{
			//HERE
			cmd->in = parse_here(cmd, sen[i + 1]);
			i++;
		}
		else if (s_iseq(sen[i], ">>"))
		{
			cmd->out = ft_strdup(sen[i + 1]);
			cmd->appp = 1;
			i++;
		}
		else
		{
			cmd->argv[j] = ft_strdup(sen[i]);
			j++;
		}
		i++;
	}
}

int	printcmd(t_cmd *cmd)
{
	int	i;

	ft_printf(" argc: %i\n", cmd->argc);
	ft_printf(" argv: ");
	i = 0;
	while (cmd->argv[i])
		ft_printf("'%s' ", cmd->argv[i++]);
	ft_printf("\n in: %s\n", cmd->in);
	ft_printf(" out: %s\n", cmd->out);
	ft_printf(" here_quoted: %i\n", cmd->here_quoted);
	return (i);
}

int	check_for_quotes(char **sen)
{
	int	i;

	if (sen[0] == NULL)
		return (1);
	i = 0;
	while (sen[i])
		i++;
	i--;
	if (char_in_set(sen[i][0], "'\""))
	{
		if (char_in_set(sen[i][ft_strlen(sen[i]) - 1], "'\""))
		{
			return (1);
		}
		else
		{
			throw_error(sen[i]);
			return (0);
		}
	}
	return (1);
}

int	sens2cmds(t_msh *m)
{
	int	i;

	m->ct->cmds = (t_cmd **)ft_calloc(1, sizeof(t_cmd *));
	i = 0;
	while (m->ct->sentences[i])
	{
		m->ct->cmds[i] = (t_cmd *)ft_calloc(1 , sizeof(t_cmd));
		ft_printf("here");
		if (!check_for_quotes(m->ct->sentences[i]))
			;
		else
		{
			compute_io(m, m->ct->sentences[i], m->ct->cmds[i]);
			printcmd(m->ct->cmds[i]);
			//TODO: Check whether everything that has to be a word is one.
		}
		i++;
	}
}

int	parse_msh(t_msh *m)
{
	int	pc;

	pc = split_by_pipes(m);
	if (pc > 1)
		pipe_checkup(m);
	sens2cmds(m);
	return (1);
}
