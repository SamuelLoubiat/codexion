/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sloubiat <sloubiat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 02:29:01 by sloubiat          #+#    #+#             */
/*   Updated: 2026/04/20 18:46:32 by sloubiat         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "header/codexion.h"

int	ft_get_time(void)
{
	struct timespec	ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (ts.tv_sec * 1000LL + ts.tv_nsec / 1000000LL);
}

int	has_burned(t_arg *arg)
{
	pthread_mutex_lock(&arg->config->mutex_burn);
	if (arg->config->burned)
	{
		pthread_mutex_unlock(&arg->config->mutex_burn);
		return (1);
	}
	pthread_mutex_unlock(&arg->config->mutex_burn);
	return (0);
}

int	has_finish(t_arg *arg, t_coders *coder)
{
	pthread_mutex_lock(&coder->coder_mutex);
	if (arg->config->number_compile <= coder->number_compile)
	{
		pthread_mutex_unlock(&coder->coder_mutex);
		return (1);
	}
	pthread_mutex_unlock(&coder->coder_mutex);
	return (0);
}

void	ft_put_str(char *str, int coder_id, t_arg *arg)
{
	pthread_mutex_lock(&arg->config->mutex_console);
	printf(str, ft_get_time() - arg->start, coder_id);
	pthread_mutex_unlock(&arg->config->mutex_console);
}
