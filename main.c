/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/17 10:23:18 by tpeters           #+#    #+#             */
/*   Updated: 2023/01/11 21:11:13 by tpeters          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

int	g_our_global;

void	handle_signal(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		if (!g_our_global)
		{
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
	}
}

int	main(int argc, char **argv)
{
	(void)argv;
	if (argc >= 2)
	{
		ft_printf("φsh: too many arguments\n");
		return (1);
	}
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	msh_loop();
	return (0);
}

char	*mk_readline(char ***env)
{
	char	*wd;
	char	*prompt;

	wd = ft_getenv(env, "PWD");
	prompt = ft_strjoin(wd, " φ ");
	ft_free((void **)&wd);
	return (prompt);
}

bool	ft_readline(t_loop *l)
{
	char	*prompt;

	if (!isatty(STDIN_FILENO))
	{
		ft_printf("Please use φshell from a terminal. Thanks :)\n");
		return (false);
	}
	prompt = mk_readline(l->env);
	if (!prompt)
		return (false);
	l->in->t = readline(prompt);
	ft_free((void **)&prompt);
	if (!l->in->t)
		return (0);
	add_history(l->in->t);
	return (1);
}

// msh_loop = execute . evaluate . parse . tokenize
void	msh_loop(void)
{
	char	**toks;
	t_msh	*m;
	t_list	*lex_lst;
	t_in	in;
	t_loop	l;

	in = (t_in){.t = NULL, .here_did_realloc = false};
	l = (t_loop){.in = &in, .rv = 0, .env = NULL};
	l.env = ft_calloc(1, sizeof(char **));
	*l.env = clone_env();
	while (ft_readline(&l) != 0)
	{
		lex_lst = lex(l.in);
		toks = list_to_arr(lex_lst);
		ft_lstclear(&lex_lst, del_toks);
		m = mk_msh(toks, l.env, l.in, &l.rv);
		if (parse_msh(m))
		{
			evaluate(m);
			execute(m);
		}
		del_msh(m);
	}
	free_strs(*l.env);
	ft_free((void **)&l.env);
}
