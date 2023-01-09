/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 12:40:03 by bhagenlo          #+#    #+#             */
/*   Updated: 2023/01/09 19:52:21 by tpeters          ###   ########.fr       */
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
# include <stdarg.h>

# define NUM_PIPES 50

extern int	g_our_global;

typedef int	t_curr_token_start;
typedef int	t_curr_token_type;

typedef struct s_mk_strlist
{
	va_list	argp;
	int		i;
	char	**lst;
	char	*str_to_add;
}	t_strlst;

typedef struct s_input
{
	char	*t;
	bool	here_did_realloc;
}	t_in;


typedef struct s_main_loop
{
	t_in	*in;
	char	***env;
	int		rv;
}	t_loop;

typedef struct s_env_helper
{
	int		i;
	char	*eqsignp;
	int		eqpos;
	char	**env;
}	t_env;

typedef struct s_replace
{
	int		i;
	int		j;
	int		k;
	int		z;
	int		newlen;
	char	*new_arg;
}	t_rep;

typedef struct s_lexer
{
	t_curr_token_start	cst;
	int					n;
	t_curr_token_type	ctt;
}	t_lex;

typedef struct s_execute_vector
{
	char	*pathname;
	char	**args;
}	t_execve;

typedef struct s_command
{
	int		argc;
	char	**argv;

	char	**args;

	char	*in;
	char	*here;
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
	t_in	*in;
	char	**toks;
	t_ct	*ct;
	char	***env;
	int		*rv;
}	t_msh;

//data.c
t_msh		*mk_msh(char **toks, char ***env, t_in *in, int *rv);
void		del_msh(t_msh *m);
void		free_all(t_msh *m);

//utils.c
void		*ft_realloc(void *ptr, size_t size);
void		*ft_reallocpl(void *ptr, size_t size, char *place);
void		ft_free(void **ptr);
void		ft_freepl(void *ptr, char *place);
int			s_iseq(char *s1, char *s2);
int			s_isneq(char *s1, char *s2, int n);
int			s_in_s(char *s, char **slist);
int			consists_of_only(char *token, char *chars);
void		ft_strcpy(char *dst, char *src);
void		serror(char *token);
int			serrorm1(char *token);
int			strslen(char **strs);
int			free_strs(char **sp);
int			free_strspl(char **sp, char *place);
void		*free_strsv(char **sp);
bool		free_strsb(char **sp);
void		prints(char **slist);
void		printns(char **slist);
bool		ft_parse_int(const char *s, int *loc);
char		**mk_strlist(int argc, ...);

//tokens.c
void		print_tokens(t_list *tokens);
void		print_tokarr(char **toks);
char		**list_to_arr(t_list *toks);

//lexer.c
t_list		*lex(t_in *in);

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
bool		extend_env(t_msh *m, char *to_add);
char		**clone_env(void);
bool		ft_setenv(t_msh *m, char *name, char *value);
char		*ft_getenv(char ***msh_env, char *varname);
int			rm_entry(t_msh *m, char *varname);

int			run_tests(void);
void		msh_loop(void);

#endif
