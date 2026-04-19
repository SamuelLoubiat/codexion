/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sloubiat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 02:56:08 by sloubiat          #+#    #+#             */
/*   Updated: 2026/04/18 20:20:03 by sloubiat         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "header/codexion.h"

static void	swap_queue(t_arg *arg, int *queue)
{
	t_coders	*curr;

	if (queue[0] > queue[1])
	{
		while (arg->coder->id != queue[1])
			arg->coder = arg->coder->next;
		curr = arg->coder;
		lock_mutex(curr);
		if (curr->last_use == curr->next->last_use
			|| curr->last_use < curr->next->last_use)
			swap(queue);
		unlock_mutex(curr);
	}
	else
	{
		while (arg->coder->id != queue[0])
			arg->coder = arg->coder->next;
		curr = arg->coder;
		lock_mutex(curr);
		if (curr->last_use > curr->next->last_use)
			swap(queue);
		unlock_mutex(curr);
	}
}

static void	sort_dongle(t_arg *arg)
{
	t_dongle	*dongle_curr;
	int			first_pass;

	first_pass = 1;
	dongle_curr = arg->dongle;
	while (dongle_curr != arg->dongle || first_pass)
	{
		first_pass = 0;
		pthread_mutex_lock(&dongle_curr->queue_mutex);
		if (dongle_curr->queue[0] != 0 && dongle_curr->queue[1] != 0)
			swap_queue(arg, dongle_curr->queue);
		pthread_mutex_unlock(&dongle_curr->queue_mutex);
		dongle_curr = dongle_curr->next;
	}
}

static int	check_burnout(t_coders *coders, t_arg *arg)
{
	pthread_mutex_lock(&coders->coder_mutex);
	if (coders->last_use + arg->config->time_burnout
		< ft_get_time() - arg->start && coders->number_compile
		!= arg->config->number_compile)
	{
		set_burnout(coders, arg);
		pthread_mutex_unlock(&coders->coder_mutex);
		return (1);
	}
	pthread_mutex_unlock(&coders->coder_mutex);
	return (0);
}

int	check_all_coders(t_arg *arg)
{
	int			end;
	int			first_pass;
	t_coders	*curr;

	end = 1;
	first_pass = 1;
	curr = arg->coder;
	while (curr != arg->coder || first_pass)
	{
		first_pass = 0;
		if (!has_finish(arg, curr))
			end = 0;
		if (check_burnout(curr, arg))
			return (1);
		curr = curr->next;
	}
	return (end);
}

void	monitor(t_arg *arg)
{
	while (1)
	{
		if (arg->config->burned)
			break ;
		if (arg->config->edf)
			sort_dongle(arg);
		if (check_all_coders(arg))
			break ;
		usleep(100);
	}
}
