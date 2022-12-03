/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/17 10:23:18 by tpeters           #+#    #+#             */
/*   Updated: 2022/12/03 18:47:16 by tpeters          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

void	handle_signal(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n"); // Move to a new line
		if (!g_our_global)
		{
			rl_on_new_line(); // Regenerate the prompt on a newline
			rl_replace_line("", 0); // Clear the previous text
			rl_redisplay();
		}
	}
}

int	main(void)
{
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	msh_loop();
	return (0);
}

// msh_loop = execute . evaluate . parse . tokenize
void	msh_loop(void)
{
	char	*t;
	char	**toks;
	t_msh	*m;
	char	***env;
	int		rv;

	rv = 0;
	env = ft_calloc(1, sizeof(char **));
	*env = clone_env();
	while(1)
	{ // is_atty ? readline : gnl
		t = readline("φ ");
		if (!t)
		{
			ft_printf("Ctrl - D caught!\n");
			return ;
		}
		add_history(t);
		toks = list_to_arr(lex(t));
		m = mk_msh(toks, env, t, &rv);
		if (parse_msh(m))
		{
			evaluate(m);
			execute(m);
			// printns(m->env);
			//print_tokens(parse_tree);
			//interpret();
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
