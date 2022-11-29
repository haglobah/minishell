/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhagenlo <bhagenlo@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 09:39:54 by bhagenlo          #+#    #+#             */
/*   Updated: 2022/11/29 21:46:09 by bhagenlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

/* void exec_cmd() */
/* { */
/* 	//save in/out */
/* 	int tmpin = dup(0); */
/* 	int tmpout = dup(1); */
/* 	//set the initial input */
/* 	int fdin; */
/* 	if (infile) { */
/* 		fdin = open(infile,O_READ); */
/* 	} */
/* 	else { */
/* 		// Use default input */
/* 		fdin=dup(tmpin); */
/* 	} */
/* 	int ret; */
/* 	int fdout; */
/* 	for(i=0;i<numsimplecommands; i++) */
/* 	{ */
/* 		//redirect input */
/* 		dup2(fdin, 0); */
/* 		close(fdin); */
/* 		//setup output */
/* 		if (i == numsimplecommands 1){ */
/* 			// Last simple command */
/* 			if(outfile){ */
/* 				fdout=open(outfile,); */
/* 			} */
/* 			else { */
/* 				// Use default output */
/* 				fdout=dup(tmpout); */
/* 			} */
/* 		} */
/* 		else */
/* 		{ */
/* 			// Not last */
/* 			//simple command */
/* 			//create pipe */
/* 			int fdpipe[2]; */
/* 			pipe(fdpipe); */
/* 			fdout=fdpipe[1]; */
/* 			fdin=fdpipe[0]; */
/* 		}// if/else */
/* 		// Redirect output */
/* 		dup2(fdout,1); */
/* 		close(fdout); */
/* 		// Create child process */
/* 		ret=fork(); */
/* 		if(ret==0) { */
/* 			execvp(scmd[i].args[0], scmd[i].args); */
/* 			perror("execvp"); */
/* 			_exit(1); */
/* 		} */
/* 	} // for */
/* 	//restore in/out defaults */
/* 	dup2(tmpin,0); */
/* 	dup2(tmpout,1); */
/* 	close(tmpin); */
/* 	close(tmpout); */
/* 	if (!background) { */
/* 		// Wait for last command */
/* 		waitpid(ret, NULL); */
/* 	} */
/* } // execute */

void	exec_cmd(t_msh *m)
{
	//if infile, read from infile
	//execute
	//if outfile, put into outfile
	//if !last, create pipe
	(void)m;
}

char **cons_args(t_cmd *cmd)
{
	int	i;
	int	j;
	int	empty_count;
	char	**args;

	empty_count = 0;
	i = -1;
	while (++i < cmd->argc)
	{
		if (s_iseq(cmd->argv[i], ""))
			empty_count += 1;
	}
	args = ft_calloc(sizeof(char *), (i - empty_count + 1));
	if (args == NULL)
		return (NULL);
	i = -1;
	j = 0;
	while (++i < cmd->argc)
	{
		if (s_iseq(cmd->argv[i], ""))
			continue ;
		args[j] = ft_calloc(sizeof(char), ft_strlen(cmd->argv[i]) + 1);
		if (args[j] == NULL)
			return (NULL);
		ft_strcpy(args[j], cmd->argv[i]);
		j++;
	}
	return (args);
}

char	*find_path(char **args)
{
	char	*path;

	path = ft_calloc(sizeof(char), ft_strlen(args[0]) + 1);
	if (path == NULL)
		return (NULL);
	//find the path (missing)
	ft_strcpy(path, args[0]);
	ft_printf("path: %s\n", path);
	return (path);
}

char	**cons_env()
{
	extern char	**environ;

	//change the env

	return (environ);
}

t_exec	*mk_exec(t_cmd *cmd)
{
	t_exec	*e;

	e = ft_calloc(sizeof(t_exec), 1);
	if (e == NULL)
		return (NULL);
	e->args = cons_args(cmd);
	// ft_printf("here\n");
	e->pathname = find_path(e->args);
	// ft_printf("%s\n", e->pathname);
	e->env = cons_env();
	return (e);
}

void	del_exec(t_exec *e)
{
	free_strs(e->args);
	free(e->pathname);
	free_strs(e->env);
	free(e);
}

int	execute_all_cmds(t_msh *m)
{
	int	tmpin;
	int	tmpout;
	int	fdin;
	int	fdout;
	int	fdpipe[2];
	int	ret;
	char	*infile;
	char	*outfile;
	t_exec	*e;
	int	i;

	tmpin = dup(0);
	tmpout = dup(1);
	i = -1;
	while (m->ct->cmds[++i])
	{
		infile = m->ct->cmds[i]->in;
		if (infile)
			fdin = open(infile, O_RDONLY);
		else
			fdin = dup(tmpin);
		dup2(fdin, 0);
		close(fdin);

		outfile = m->ct->cmds[i]->out;
		if (outfile)
			fdout = open(outfile, O_WRONLY | (m->ct->cmds[i]->appp) ? O_APPEND : 0);
		else
		{
			if (m->ct->cmds[i + 1] == NULL)
				fdout = dup(tmpout);
			else
			{
				if (pipe(fdpipe) < 0)
					ft_printf("pipecreation failed\n");
				fdout = fdpipe[1];
				fdin = fdpipe[0];
			}
		}
		dup2(fdout, 1);
		close(fdout);

		ret = fork();
		if (ret == 0)
		{
			ft_printf("I exist: pid %i\n", getpid());
			e = mk_exec(m->ct->cmds[i]);
			if (e == NULL)
				return (1);
			if (execve(e->pathname, e->args, e->env) == -1)
				ft_printf("execve failed.\n");
			free(e);
		}
		else if (ret == -1)
			perror("fork failed\n");

		//restore IO defaults
		dup2(tmpin, 0);
		dup2(tmpout, 1);

	}
	close(tmpin);
	close(tmpout);
	waitpid(ret, NULL, 0);
	//return the error code?
	return (0);
}

int	execute(t_msh *m)
{
	execute_all_cmds(m);
	return (1);
}
