/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sloubiat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 03:46:50 by sloubiat          #+#    #+#             */
/*   Updated: 2026/04/20 18:19:19 by sloubiat         ###   ########lyon.fr   */
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

void	lock_mutex(t_coders *coders)
{
	pthread_mutex_lock(&coders->coder_mutex);
	pthread_mutex_lock(&coders->next->coder_mutex);
}

void	unlock_mutex(t_coders *coders)
{
	pthread_mutex_unlock(&coders->coder_mutex);
	pthread_mutex_unlock(&coders->next->coder_mutex);
}

void	set_burnout(t_coders *coders, t_arg *arg)
{
	pthread_mutex_lock(&arg->config->mutex_burn);
	arg->config->burned = 1;
	ft_put_str("%d %d burned out\n", coders->id, arg);
	pthread_mutex_unlock(&arg->config->mutex_burn);
}
