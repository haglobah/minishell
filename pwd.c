/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhagenlo <bhagenlo@student.42heil...>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 16:08:27 by bhagenlo          #+#    #+#             */
/*   Updated: 2022/12/01 16:08:27 by bhagenlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

int	pwd()
{
	ft_printf("%s\n", getenv("PWD"));
	return (0);
}

int	main()
{
	pwd();
}
