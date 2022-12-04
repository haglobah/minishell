/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 12:40:03 by bhagenlo          #+#    #+#             */
/*   Updated: 2022/12/04 02:01:23 by tpeters          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_H

# define MSH_H

# include "libft/libft.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>

# define NUM_PIPES 50

extern int	g_our_global;

typedef int	current_token_start;
typedef int	current_token_type;

typedef struct s_lexer
{
	current_token_start	cst;
	int					n;
	current_token_type	ctt;
}	t_lex;

typedef struct s_execute_vector
{
	char	*pathname;
	char	**args;
	char	**env;
}	t_execve;

typedef struct s_command
{
	int		argc;
	char	**argv;

//	NULL-terminated
	char	**args;

	char	*in;
	char	*out;
	int		appp;
	int		here_quoted;
}	t_cmd;

typedef struct s_command_table
{
	t_cmd	**cmds;
	char	***sentences;
	int		senc;
}	t_ct;

//data.c
t_ct		*mk_ct(void);
void		del_ct(t_ct *ct);

typedef struct s_minishell
{
	char	*t;
	char	**toks;
	t_ct	*ct;
	char	***env;
	int		*rv;
}	t_msh;

//data.c
t_msh		*mk_msh(char **toks, char ***env, char *t, int *rv);
void		del_msh(t_msh *m);

//utils.c
void		*ft_realloc(void *ptr, size_t size);
int			s_iseq(char *s1, char *s2);
int			s_isneq(char *s1, char *s2, int n);
int			s_in_s(char *s, char **slist);
int			consists_of_only(char *token, char *chars);
void		ft_strcpy(char *dst, char *src);
void		serror(char *token);
int			strslen(char **strs);
int			free_strs(char **sp);
void		prints(char **slist);
void		printns(char **slist);
bool		ft_parse_int(const char *s, int *loc);

//tokens.c
void		print_tokens(t_list *tokens);
void		print_tokarr(char **toks);
char		**list_to_arr(t_list *toks);

//lexer.c
t_list		*lex(char *in);

//parser.c
int			parse_msh(t_msh *m);
char		*parse_here(t_cmd *cmd, char *here);
int			printcmd(t_cmd *cmd);

//evaluator.c
int			evaluate(t_msh *m);

//execve.c
t_execve	*mk_execve(t_msh *m, t_cmd *cmd);
void		del_execve(t_execve *ev);

//executor.c
char		**cons_args(t_cmd *cmd);
int			execute(t_msh *m);

//builtins.c
int			ft_echo(t_msh *m, char **args);
int			ft_cd(t_msh *m, char **args);
int			ft_pwd(t_msh *m, char **args);
int			ft_export(t_msh *m, char **args);
int			ft_unset(t_msh *m, char **args);
int			ft_env(t_msh *m, char **args);
int			ft_exit(t_msh *m, char **args);

//env.c
bool		extend_env(char ***env, char *to_add);
char		**clone_env(void);
bool		ft_setenv(t_msh *m, char *name, char *value);
char		*ft_getenv(t_msh *m, char *varname);
int			rm_entry(t_msh *m, char *varname);

int			run_tests(void);
void		msh_loop(void);

#endif
