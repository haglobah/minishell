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

int	printt(int passed, int total)
{
	return (ft_printf("\nPassed %i / Failed %i (of %i)\n", passed,
					  total - passed, total));
}

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
//		ft_printf("\'%s\', ", str_to_add);
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

	total_tests = 0;
	pass_count = 0;
	ft_printf("\nTesting Lexer...\n");
	pass_count += lst_iseq(lex(""),
						   mk_strlist(1, ""),
						   &total_tests);
	pass_count += lst_iseq(lex("hello>"),
						   mk_strlist(2, "hello", ">"),
						   &total_tests);
	pass_count += lst_iseq(lex("hello|bla"),
						   mk_strlist(3, "hello", "|", "bla"),
						   &total_tests);
	pass_count += lst_iseq(lex("hello|bla<infile"),
						   mk_strlist(5, "hello", "|", "bla", "<", "infile"),
						   &total_tests);
	/* pass_count += lst_iseq(lex("hello|bla<<HERE\nbla\nasdf\nHERE"), */
	/* 					   mk_strlist(5, "hello", "|", "bla", "<<", "HERE\nbla\nasdf\nHERE"), */
	/* 					   &total_tests); */
	printt(pass_count, total_tests);
}

int	int_iseq(int parsed, int expected, int *num_tests)
{
	*num_tests += 1;
	if (parsed != expected)
	{
		ft_printf("parse: Test %i failed.", *num_tests);
	}
	return (parsed == expected);
}

int	parse(char *t)
{
	return (parse_msh(mk_msh(list_to_arr(lex(t)))));
}

void	test_parser()
{
	int	total_tests;
	int	passed;

	total_tests = 0;
	passed = 0;
	ft_printf("\nTesting Parser...\n");
	passed += int_iseq(parse("bla"), 1, &total_tests);
	passed += int_iseq(parse("'hello "), 0, &total_tests);
	passed += int_iseq(parse("I < am > a \" crazy \" | string"), 1, &total_tests);
	passed += int_iseq(parse("\" lala "), 0, &total_tests);
	passed += int_iseq(parse("|"), 0, &total_tests);
	printt(passed, total_tests);
}

int	run_tests(void)
{
	test_lexer();
	test_parser();
	return (0);
}
