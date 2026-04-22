/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_dongle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sloubiat <sloubiat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 15:13:00 by sloubiat          #+#    #+#             */
/*   Updated: 2026/04/22 15:13:01 by sloubiat         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "header/codexion.h"

int	set_dongle_default(t_dongle *dongle, int *id)
{
	if (!dongle)
		return (0);
	dongle->id = (*id)++;
	dongle->last_use = 0;
	dongle->queue[0] = 0;
	dongle->queue[1] = 0;
	dongle->prev = 0;
	dongle->next = 0;
	if (pthread_mutex_init(&dongle->mutex, NULL))
		return (0);
	if (pthread_mutex_init(&dongle->queue_mutex, NULL))
		return (0);
	return (1);
}

int	init_data_dongle(t_dongle *dongles, t_dongle *actual, int *id)
{
	actual->next = malloc(sizeof(t_dongle));
	if (!set_dongle_default(actual->next, id))
	{
		free_dongles(dongles);
		return (0);
	}
	actual->next->prev = actual;
	return (1);
}
