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

#define NUM_PIPES 50

typedef struct s_command
{
	int	argc;
	char	**argv;

	char	*in;
	char	*out;
	int	appp;
}	t_cmd;

typedef struct s_command_table
{
	t_cmd	**cmds;
	char	***sentences;

	int	senc;
}	t_ct;

t_ct	*mk_ct();
void	del_ct(t_ct *ct);

typedef struct s_minishell
{
	char	**toks;
	t_ct	*ct;
}	t_msh;

t_msh	*mk_msh(char **toks);
void	del_msh(t_msh *m);

void	*ft_realloc(void *ptr, size_t size);
int	s_iseq(char *s1, char *s2);
int	s_isneq(char * s1, char *s2, int n);
int	consists_of_only(char *token, char *chars);

t_list	*lex(char *in);
int	parse_msh(t_msh *m);

int	run_tests(void);
void	msh_loop(void);
