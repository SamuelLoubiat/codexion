/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sloubiat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 03:46:50 by sloubiat          #+#    #+#             */
/*   Updated: 2026/04/22 00:18:41 by sloubiat         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "header/codexion.h"

void	swap(int *queue)
{
	int	temp;

	temp = queue[0];
	queue[0] = queue[1];
	queue[1] = temp;
}

void	set_burnout(t_coders *coders, t_arg *arg)
{
	pthread_mutex_lock(&arg->config->mutex_burn);
	arg->config->burned = 1;
	pthread_mutex_unlock(&arg->config->mutex_burn);
	ft_put_str("%d %d burned out\n", coders->id, arg);
}

void	set_queue(t_dongle *dongle_curr, t_coders *curr, int reverse)
{
	if (reverse)
	{
		dongle_curr->queue[1] = curr->id;
		dongle_curr->queue[0] = curr->next->id;
	}
	else
	{
		dongle_curr->queue[0] = curr->id;
		dongle_curr->queue[1] = curr->next->id;
	}
}

void	set_queue_order(t_coders *curr, t_dongle *dongle_curr)
{
	int			last_use;
	int			last_use_next;

	pthread_mutex_lock(&curr->coder_mutex);
	last_use = curr->last_use;
	pthread_mutex_unlock(&curr->coder_mutex);
	pthread_mutex_lock(&curr->next->coder_mutex);
	last_use_next = curr->next->last_use;
	pthread_mutex_unlock(&curr->next->coder_mutex);
	pthread_mutex_lock(&dongle_curr->queue_mutex);
	if (last_use == last_use_next)
	{
		if (curr->id % 2 == 0)
			set_queue(dongle_curr, curr, 0);
		else
			set_queue(dongle_curr, curr, 1);
	}
	else if (last_use < last_use_next)
		set_queue(dongle_curr, curr, 0);
	else
		set_queue(dongle_curr, curr, 1);
	pthread_mutex_unlock(&dongle_curr->queue_mutex);
}
