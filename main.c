/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/17 10:23:18 by tpeters           #+#    #+#             */
/*   Updated: 2022/11/25 16:19:23 by tpeters          ###   ########.fr       */
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
//	export();
	return (0);
}
