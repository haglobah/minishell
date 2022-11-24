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

#ifndef MSH_H

# define MSH_H

# include "libft/libft.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>

# define NUM_PIPES 50

typedef int current_token_start;
typedef int current_token_type;

typedef struct s_lexer
{
	current_token_start	cst;
	int					n;
	current_token_type	ctt;
}	t_lex;

typedef struct s_execute
{
	char	*pathname;
	char	**args;
	char	**env;
}	t_exec;

typedef struct s_command
{
	int	argc;
	char	**argv;

	char	*in;
	char	*out;
	int	appp;
	int	here_quoted;
}	t_cmd;

typedef struct s_command_table
{
	t_cmd	**cmds;
	char	***sentences;

	int	senc;
}	t_ct;

//data.c
t_ct	*mk_ct();
void	del_ct(t_ct *ct);

typedef struct s_minishell
{
	char	**toks;
	t_ct	*ct;
}	t_msh;

t_msh	*mk_msh(char **toks);
void	del_msh(t_msh *m);

//utils.c
void	*ft_realloc(void *ptr, size_t size);
int	s_iseq(char *s1, char *s2);
int	s_isneq(char * s1, char *s2, int n);
int	s_in_s(char *s, char **slist);
int	consists_of_only(char *token, char *chars);
void	ft_strcpy(char *dst, char *src);
void	serror(char *token);
void	free_strs(char **sp);
void	prints(char **slist);

//tokens.c
void	print_tokens(t_list *tokens);
void	print_tokarr(char **toks);
char	**list_to_arr(t_list *toks);

//lexer
t_list	*lex(char *in);

//parser.c
int	parse_msh(t_msh *m);
char	*parse_here(t_cmd *cmd, char *here);
int	printcmd(t_cmd *cmd);

//evaluator.c
int	evaluate(t_msh *m);

//executor.c
int	execute(t_msh *m);
t_exec	*mk_exec(t_cmd *cmd);
void	del_exec(t_exec *e);

int	run_tests(void);
void	msh_loop(void);

#endif
