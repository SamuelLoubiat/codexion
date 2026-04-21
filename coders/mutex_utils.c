/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sloubiat <sloubiat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 01:34:22 by sloubiat          #+#    #+#             */
/*   Updated: 2026/04/18 01:34:25 by sloubiat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header/codexion.h"

void	register_mutex(t_dongle *dongle, t_coders *coder)
{
	if (dongle->queue[0] == 0)
		dongle->queue[0] = coder->id;
	else if (dongle->queue[1] == 0)
		dongle->queue[1] = coder->id;
}

int	mutex_lock(t_dongle *dongle, t_arg *arg, t_coders *coder)
{
	long long	now;

	pthread_mutex_lock(&dongle->queue_mutex);
	if (dongle->queue[0] != coder->id)
	{
		pthread_mutex_unlock(&dongle->queue_mutex);
		return (0);
	}
	pthread_mutex_unlock(&dongle->queue_mutex);
	pthread_mutex_lock(&dongle->mutex);
	now = ft_get_time() - arg->start;
	if (dongle->last_use != 0
		&& now < dongle->last_use + arg->config->dongle_cooldown)
	{
		pthread_mutex_unlock(&dongle->mutex);
		return (0);
	}
	pthread_mutex_lock(&dongle->queue_mutex);
	dongle->queue[0] = dongle->queue[1];
	dongle->queue[1] = 0;
	pthread_mutex_unlock(&dongle->queue_mutex);
	return (1);
}

void	mutex_unlock(t_dongle *dongle, t_arg *arg)
{
	dongle->last_use = ft_get_time() - arg->start;
	pthread_mutex_unlock(&dongle->mutex);
}
