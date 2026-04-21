/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sloubiat <sloubiat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 19:34:50 by sloubiat          #+#    #+#             */
/*   Updated: 2026/04/21 19:41:37 by sloubiat         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"

void	find_dongle(t_dongle **first, t_dongle **second, t_arg *arg)
{
	*first = arg->dongle;
	while ((*first)->id != arg->coder->id)
		*first = (*first)->next;
	*second = *first;
	*first = (*second)->prev;
	if (arg->coder->id % 2 == 0)
	{
		*second = *first;
		*first = (*first)->next;
	}
}
