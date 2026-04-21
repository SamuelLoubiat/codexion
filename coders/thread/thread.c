/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sloubiat <sloubiat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 16:41:47 by sloubiat          #+#    #+#             */
/*   Updated: 2026/04/20 18:46:06 by sloubiat         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"

int	take_dongle(t_arg *arg, t_coders *coders, t_dongle *dongles)
{
	while (!mutex_lock(dongles, arg, coders))
	{
		if (has_burned(arg))
			return (0);
		usleep(100);
	}
	if (has_burned(arg))
	{
		pthread_mutex_unlock(&dongles->mutex);
		return (0);
	}
	ft_put_str("%d %d has taken a dongle\n", coders->id, arg);
	return (1);
}

void	compile(t_coders *coders, t_arg *arg)
{
	ft_put_str("%d %d is compiling\n", coders->id, arg);
	pthread_mutex_lock(&coders->coder_mutex);
	coders->last_use = ft_get_time() - arg->start;
	if (coders->last_use == 0)
		coders->last_use = 1;
	pthread_mutex_unlock(&coders->coder_mutex);
	usleep((unsigned int) arg->config->time_compile * 1000);
	pthread_mutex_lock(&coders->coder_mutex);
	coders->number_compile += 1;
	pthread_mutex_unlock(&coders->coder_mutex);
}

int	select_dongle(t_arg *arg, t_dongle *first, t_dongle *second)
{
	if (first == second)
		return (0);
	pthread_mutex_lock(&first->queue_mutex);
	pthread_mutex_lock(&second->queue_mutex);
	register_mutex(first, arg->coder);
	register_mutex(second, arg->coder);
	pthread_mutex_unlock(&first->queue_mutex);
	pthread_mutex_unlock(&second->queue_mutex);
	if (!take_dongle(arg, arg->coder, first))
		return (0);
	if (!take_dongle(arg, arg->coder, second))
	{
		pthread_mutex_unlock(&first->mutex);
		return (0);
	}
	compile(arg->coder, arg);
	mutex_unlock(second, arg);
	mutex_unlock(first, arg);
	return (1);
}

int	debug_refactor(t_arg *arg, t_coders *coders)
{
	if (has_burned(arg))
		return (0);
	ft_put_str("%d %d is debugging\n", coders->id, arg);
	usleep((unsigned int) arg->config->time_debug * 1000);
	if (has_burned(arg))
		return (0);
	ft_put_str("%d %d is refactoring\n", coders->id, arg);
	usleep((unsigned int) arg->config->time_refactor * 1000);
	return (1);
}

void	thread(t_arg *arg)
{
	t_dongle	*first;
	t_dongle	*second;

	first = arg->dongle;
	while (first->id != arg->coder->id)
		first = first->next;
	second = first;
	first = second->prev;
	if (arg->coder->id % 2 == 0)
	{
		second = first;
		first = first->next;
	}
	while (1)
	{
		if (has_burned(arg))
			return ;
		if (!select_dongle(arg, first, second))
		{
			free(arg);
			return ;
		}
		pthread_mutex_lock(&arg->config->mutex_burn);
		if (arg->config->end)
		{
			free(arg);
			pthread_mutex_unlock(&arg->config->mutex_burn);
			return ;
		}
		pthread_mutex_unlock(&arg->config->mutex_burn);
		if (!debug_refactor(arg, arg->coder))
		{
			free(arg);
			return ;
		}
	}
}
