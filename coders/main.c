/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sloubiat <sloubiat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 18:06:02 by sloubiat          #+#    #+#             */
/*   Updated: 2026/04/23 14:41:43 by sloubiat         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "header/codexion.h"

int	start_coders(t_config *config, t_coders *coders, t_dongle *dongles,
	t_arg *arg)
{
	arg->coder = coders;
	arg->dongle = dongles;
	arg->config = config;
	if (pthread_create(&coders->thread, NULL, (void *) thread, arg) != 0)
	{
		pthread_mutex_lock(&config->mutex_burn);
		config->burned = 1;
		pthread_mutex_unlock(&config->mutex_burn);
		free(arg);
		return (0);
	}
	return (1);
}

int	execute_all(t_coders *coders, t_dongle *dongles,
	long long start, t_config *config)
{
	t_coders	*curr;
	t_arg		*arg;
	int			first_pass;

	first_pass = 1;
	curr = coders;
	while (curr != coders || first_pass)
	{
		first_pass = 0;
		arg = malloc(sizeof(t_arg));
		if (!arg)
		{
			pthread_mutex_lock(&config->mutex_burn);
			config->burned = 1;
			pthread_mutex_unlock(&config->mutex_burn);
			return (0);
		}
		arg->start = start;
		if (!start_coders(config, curr, dongles, arg))
			return (0);
		curr = curr->next;
	}
	return (1);
}

void	end(t_arg *arg, pthread_t	monitor_tread)
{
	int			first_pass;
	t_coders	*curr;

	first_pass = 1;
	curr = arg->coder;
	while (curr != arg->coder || first_pass)
	{
		first_pass = 0;
		if (curr->thread)
			pthread_join(curr->thread, NULL);
		curr = curr->next;
	}
	pthread_join(monitor_tread, NULL);
	free_all(arg->config, arg->dongle, arg->coder);
}

int	load_config(int argc, char **argv, t_arg *arg)
{
	arg->config = parse(argc, argv);
	if (!arg->config)
		return (0);
	arg->start = ft_get_time();
	arg->coder = init_coders(arg->config->number_coders - 1);
	if (!arg->coder)
	{
		free_config(arg->config);
		return (0);
	}
	arg->dongle = init_dongles(arg->config->number_coders - 1);
	if (!arg->dongle)
	{
		free_coders(arg->coder);
		free_config(arg->config);
		return (0);
	}
	return (1);
}

int	main(int argc, char **argv)
{
	pthread_t	monitor_tread;
	t_arg		arg;

	if (!load_config(argc, argv, &arg))
	{
		printf("Error\n");
		return (0);
	}
	if (arg.config->number_compile == 0)
	{
		free_all(arg.config, arg.dongle, arg.coder);
		return (0);
	}
	if (arg.config->edf)
		new_sorter(&arg);
	if (pthread_create(&monitor_tread, NULL, (void *) monitor, &arg) == 0)
	{
		if (!execute_all(arg.coder, arg.dongle, arg.start, arg.config))
			printf("Error\n");
	}
	else
		printf("Error\n");
	end(&arg, monitor_tread);
	return (0);
}
