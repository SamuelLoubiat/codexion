/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sloubiat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 02:56:08 by sloubiat          #+#    #+#             */
/*   Updated: 2026/04/20 18:46:22 by sloubiat         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "header/codexion.h"

static t_coders *getcoder(int id, t_arg *arg)
{
	t_coders  *curr;

	curr = arg->coder;
	while (curr->id != id)
		curr = curr->next;
	return curr;
}

void	new_sorter(t_arg *arg)
{
	t_dongle	*dongle_curr;
	t_coders	*curr;
	int			first_pass;

	first_pass = 1;
	dongle_curr = arg->dongle;
	if (dongle_curr == dongle_curr->next)
		return ;
	curr = getcoder(dongle_curr->id, arg);
	while (dongle_curr != arg->dongle || first_pass)
	{
		pthread_mutex_lock(&curr->coder_mutex);
		pthread_mutex_lock(&curr->next->coder_mutex);
		pthread_mutex_lock(&dongle_curr->queue_mutex);
		if (curr->last_use == curr->next->last_use)
		{
			if (curr->id < curr->next->id)
			{
				dongle_curr->queue[0] = curr->id;
				dongle_curr->queue[1] = curr->next->id;
			}
			else
			{
				dongle_curr->queue[1] = curr->id;
				dongle_curr->queue[0] = curr->next->id;
			}
		}
		else if (curr->last_use < curr->next->last_use)
		{
			dongle_curr->queue[0] = curr->id;
			dongle_curr->queue[1] = curr->next->id;
		}
		else {
			dongle_curr->queue[1] = curr->id;
			dongle_curr->queue[0] = curr->next->id;
		}
		pthread_mutex_unlock(&dongle_curr->queue_mutex);
		pthread_mutex_unlock(&curr->coder_mutex);
		pthread_mutex_unlock(&curr->next->coder_mutex);
		first_pass = 0;
		dongle_curr = dongle_curr->next;
		curr = curr->next;
	}
}

static int	check_burnout(t_coders *coders, t_arg *arg)
{
	pthread_mutex_lock(&coders->coder_mutex);
	if (coders->last_use + arg->config->time_burnout
		< ft_get_time() - arg->start)
	{
		set_burnout(coders, arg);
		pthread_mutex_unlock(&coders->coder_mutex);
		return (-1);
	}
	pthread_mutex_unlock(&coders->coder_mutex);
	return (0);
}

int	check_all_coders(t_arg *arg)
{
	int			end;
	int			first_pass;
	t_coders	*curr;

	end = 0;
	first_pass = 1;
	curr = arg->coder;
	while (curr != arg->coder || first_pass)
	{
		first_pass = 0;
		if (check_burnout(curr, arg) == -1)
			return (-1);
		if (has_finish(arg, curr))
			end += 1;
		curr = curr->next;
	}
	return (end);
}

void	monitor(t_arg *arg)
{
	int	interupt;

	while (1)
	{
		interupt = check_all_coders(arg);
		if (interupt == -1)
			break;
		if (arg->config->edf)
			new_sorter(arg);
		if (interupt > 0)
		{
			pthread_mutex_lock(&arg->config->mutex_burn);
			arg->config->end = 1;
			pthread_mutex_unlock(&arg->config->mutex_burn);
		}
		if (interupt == arg->config->number_coders)
			break ;
		usleep(100);
	}
}
