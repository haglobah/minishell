/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhagenlo <bhagenlo@student.42heil...>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 12:38:34 by bhagenlo          #+#    #+#             */
/*   Updated: 2022/09/21 12:38:34 by bhagenlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"
#include <stdarg.h>

t_list	*mk_strlist(int argc, ...)
{
	va_list	argp;
	int		i;
	t_list		*res;

	res = NULL;
	va_start(argp, argc);
	i = 0;
	while(i < argc)
	{
		char *str_to_add = va_arg(argp, char *);
		ft_lstadd_back(&res, ft_lstnew(ft_strdup(str_to_add)));
		//ft_printf("\'%s\', ", str_to_add);
		i++;
	}
	va_end(argp);
	return (res);
}

int	lst_iseq(t_list *to_test, t_list *check, int *num_tests)
{
	*num_tests += 1;
	while(to_test)
	{
		if (ft_strcmp(to_test->content, check->content) != 0)
		{
			ft_printf("Test %i failed: \'%s\' ≠ \'%s\'\n", *num_tests, to_test->content, check->content);
			return (0);
		}
		to_test = to_test->next;
		check = check->next;
	}
	return (1);
}

void	test_lexer()
{
	int	total_tests;
	int	pass_count;
	int	i;

	total_tests = 0;
	pass_count = 0;
	i = 0;
	ft_printf("\nTesting Lexer...\n");
	pass_count += lst_iseq(lex(""),
						   mk_strlist(1, ""),
						   &total_tests);
	pass_count += lst_iseq(lex("hello>"),
						   mk_strlist(2, "hello", ">"),
						   &total_tests);
	pass_count += lst_iseq(lex("hello|bla"),
						   mk_strlist(2, "hello", "|", "bla"),
						   &total_tests);
	ft_printf("\nPassed %i / Failed %i (of %i)\n",
			  pass_count,
			  total_tests - pass_count,
			  total_tests);
}

void	test_parser()
{
	int	total_tests;
	int	pass_count;
	int	i;

	total_tests = 0;
	pass_count = 0;
	i = 0;
	ft_printf("\nTesting Parser...\n");
	pass_count +=
	ft_printf("\nPassed %i / Failed %i (of %i)\n",
			  pass_count,
			  total_tests - pass_count,
			  total_tests);
}

int	run_tests(void)
{
	test_lexer();
}
