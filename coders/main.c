/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sloubiat <sloubiat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 18:06:02 by sloubiat          #+#    #+#             */
/*   Updated: 2026/04/13 18:06:03 by sloubiat         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "header/codexion.h"

void	test(t_arg *arg)
{
	t_dongle	*dongles;
	t_dongle	*first;
	t_dongle	*second;
	t_coders	*coders;

	if (has_burned(arg))
		return ;
	if (arg->config->number_compile == arg->coder->number_compile)
	{
		free(arg);
		return ;
	}
	dongles = arg->dongle;
	coders = arg->coder;
	while (dongles->id != coders->id)
		dongles = dongles->next;
	if (coders->id % 2 == 0)
	{
		first = dongles->prev;
		second = dongles;
	}
	else
	{
		first = dongles;
		second = dongles->prev;
	}
	register_mutex(first, coders);
	register_mutex(second, coders);
	while (!mutex_lock(first, arg, coders))
	{
		if (has_burned(arg))
			return ;
		usleep(100);
	}
	if (has_burned(arg))
	{
		pthread_mutex_unlock(&first->mutex);
		return ;
	}
	ft_put_str("%d %d has taken a dongle\n",
		coders->id, ft_get_time() - arg->start, arg);
	while (!mutex_lock(second, arg, coders))
	{
		if (has_burned(arg))
		{
			pthread_mutex_unlock(&first->mutex);
			return ;
		}
		usleep(100);
	}
	if (has_burned(arg))
	{
		pthread_mutex_unlock(&first->mutex);
		pthread_mutex_unlock(&second->mutex);
		return ;
	}
	ft_put_str("%d %d has taken a dongle\n",
		coders->id, ft_get_time() - arg->start, arg);
	ft_put_str("%d %d is compiling\n",
		coders->id, ft_get_time() - arg->start, arg);
	pthread_mutex_lock(&coders->coder_mutex);
	coders->last_use = ft_get_time() - arg->start;
	pthread_mutex_unlock(&coders->coder_mutex);
	usleep((unsigned int) arg->config->time_compile * 1000);
	pthread_mutex_lock(&coders->coder_mutex);
	coders->number_compile += 1;
	pthread_mutex_unlock(&coders->coder_mutex);
	mutex_unlock(first, arg);
	mutex_unlock(second, arg);
	if (has_burned(arg))
		return ;
	ft_put_str("%d %d is debugging\n",
		coders->id, ft_get_time() - arg->start, arg);
	usleep((unsigned int) arg->config->time_debug * 1000);
	if (has_burned(arg))
		return ;
	ft_put_str("%d %d is refactoring\n",
		coders->id, ft_get_time() - arg->start, arg);
	usleep((unsigned int) arg->config->time_refactor * 1000);
	if (coders->number_compile < arg->config->number_compile)
		test(arg);
	else
		free(arg);
}

void	execute_all(t_coders *coders, t_dongle *dongles,
	long long start, t_config *config)
{
	t_coders	*curr;
	t_arg		*arg;

	curr = coders->next;
	arg = malloc(sizeof(t_arg));
	arg->coder = coders;
	arg->dongle = dongles;
	arg->start = start;
	arg->config = config;
	pthread_create(&coders->thread, NULL, (void *) test, arg);
	while (curr != coders)
	{
		arg = malloc(sizeof(t_arg));
		arg->coder = curr;
		arg->dongle = dongles;
		arg->start = start;
		arg->config = config;
		pthread_create(&curr->thread, NULL, (void *) test, arg);
		curr = curr->next;
	}
}

int	main(int argc, char **argv)
{
	long long	start;
	t_config	*config;
	t_coders	*coders;
	t_dongle	*dongles;
	pthread_t	monitor_tread;
	t_arg		arg;
	t_coders	*temp;

	start = ft_get_time();
	config = parse(argc, argv);
	if (!config)
	{
		printf("Error");
		return (0);
	}
	coders = init_coders(config->number_coders);
	dongles = init_dongles(config->number_coders);
	arg.config = config;
	arg.start = start;
	arg.coder = coders;
	arg.dongle = dongles;
	pthread_create(&monitor_tread, NULL, (void *) monitor, &arg);
	execute_all(coders, dongles, start, config);
	pthread_join(coders->thread, NULL);
	temp = coders->next;
	while (temp != coders)
	{
		pthread_join(temp->thread, NULL);
		temp = temp->next;
	}
	pthread_join(monitor_tread, NULL);
	pthread_mutex_destroy(&config->mutex_console);
	pthread_mutex_destroy(&config->mutex_burn);
	free(config);
	free_coders(coders);
	free_dongles(dongles);
	return (0);
}
