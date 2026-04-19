/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sloubiat <sloubiat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 16:41:47 by sloubiat          #+#    #+#             */
/*   Updated: 2026/04/19 17:56:03 by sloubiat         ###   ########lyon.fr   */
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
	ft_put_str("%d %d has taken a dongle\n",
		coders->id, ft_get_time() - arg->start, arg);
	return (1);
}

void	compile(t_coders *coders, t_arg *arg)
{
	ft_put_str("%d %d is compiling\n",
		coders->id, ft_get_time() - arg->start, arg);
	pthread_mutex_lock(&coders->coder_mutex);
	coders->last_use = ft_get_time() - arg->start;
	pthread_mutex_unlock(&coders->coder_mutex);
	usleep((unsigned int) arg->config->time_compile * 1000);
	pthread_mutex_lock(&coders->coder_mutex);
	coders->number_compile += 1;
	pthread_mutex_unlock(&coders->coder_mutex);
}

int	select_dongle(t_arg *arg, t_dongle *first, t_dongle *second)
{
	register_mutex(first, arg->coder);
	register_mutex(second, arg->coder);
	if (!take_dongle(arg, arg->coder, first))
		return (0);
	if (!take_dongle(arg, arg->coder, second))
	{
		pthread_mutex_unlock(&first->mutex);
		return (0);
	}
	compile(arg->coder, arg);
	mutex_unlock(first, arg);
	mutex_unlock(second, arg);
	return (1);
}

int	debug_refactor(t_arg *arg, t_coders *coders)
{
	if (has_burned(arg))
		return (0);
	ft_put_str("%d %d is debugging\n",
		coders->id, ft_get_time() - arg->start, arg);
	usleep((unsigned int) arg->config->time_debug * 1000);
	if (has_burned(arg))
		return (0);
	ft_put_str("%d %d is refactoring\n",
		coders->id, ft_get_time() - arg->start, arg);
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
	second = first->prev;
	if (arg->coder->id % 2 == 0)
	{
		second = first;
		first = first->prev;
	}
	if (has_burned(arg))
		return ;
	if (arg->config->number_compile == arg->coder->number_compile
		|| !select_dongle(arg, first, second)
		|| !debug_refactor(arg, arg->coder))
	{
		free(arg);
		return ;
	}
	if (arg->coder->number_compile < arg->config->number_compile)
		thread(arg);
	else
		free(arg);
}
