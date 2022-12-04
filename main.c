/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/17 10:23:18 by tpeters           #+#    #+#             */
/*   Updated: 2022/12/04 16:54:33 by tpeters          ###   ########.fr       */
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
	free(wd);
	return (prompt);
}


void	del_toks(void *content)
{
	free(content);
}

// msh_loop = execute . evaluate . parse . tokenize
void	msh_loop(void)
{
	char	*t;
	char	**toks;
	t_msh	*m;
	char	***env;
	char	*prompt;
	int		rv;
	t_list	*lex_lst;

	rv = 0;
	env = ft_calloc(1, sizeof(char **));
	*env = clone_env();
	while (1)
	{
		if (!isatty(STDIN_FILENO))
		{
			ft_printf("Please use φshell from a terminal. Thanks :)\n");
			// free_all(m);
			return ;
		}
		prompt = mk_readline(env);
		if (!prompt)
			break ;
		t = readline(prompt);
		free(prompt);
		if (!t)
		{
			ft_printf("Ctrl - D caught!\n");
			break ;
		}
		add_history(t);
		lex_lst = lex(t);
		toks = list_to_arr(lex_lst);
		ft_lstclear(&lex_lst, del_toks);
		m = mk_msh(toks, env, t, &rv);
		if (parse_msh(m))
		{
			evaluate(m);
			execute(m);
		}
		else
		{
			/* ft_printf("Bad Command!\n"); */
		}
		del_msh(m);
	}
	free_strs(*env);
	free(env);
}
