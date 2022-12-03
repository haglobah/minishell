/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhagenlo <bhagenlo@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/17 10:23:18 by tpeters           #+#    #+#             */
/*   Updated: 2022/12/03 15:29:01 by bhagenlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

void	handle_signal(int sig)
{
	ft_printf("sig caught: %d\n", sig);
	if (sig == SIGINT)
	{
		//show prompt again
		//(kill all subsequent processes?)
	}
}

int	main(void)
{
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, handle_signal);
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
		m = mk_msh(toks, env);
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
		free(t);
		del_msh(m);
	}
	free_strs(*env);
	free(env);
}
