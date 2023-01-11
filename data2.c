/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpeters <tpeters@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 20:54:32 by tpeters           #+#    #+#             */
/*   Updated: 2023/01/11 21:11:23 by tpeters          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"

void	free_all(t_msh *m)
{
	free_strs(*m->env);
	ft_free((void **)&m->env);
	del_msh(m);
}

void	del_toks(void *content)
{
	ft_free((void **)&content);
}
