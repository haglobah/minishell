/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhagenlo <bhagenlo@student.42heil...>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 12:40:03 by bhagenlo          #+#    #+#             */
/*   Updated: 2022/09/21 12:40:03 by bhagenlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_minishell
{
	char	**toks;
}	t_msh;

t_msh	*mk_msh(char **toks);
void	del_msh(t_msh *m);

t_list	*lex(char *in);
int	parse(t_msh *m);

int	run_tests(void);
void	msh_loop(void);
