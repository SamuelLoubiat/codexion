/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sloubiat <sloubiat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 18:06:02 by sloubiat          #+#    #+#             */
/*   Updated: 2026/04/19 17:55:51 by sloubiat         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "header/codexion.h"

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
		arg->coder = curr;
		arg->dongle = dongles;
		arg->start = start;
		arg->config = config;
		pthread_create(&curr->thread, NULL, (void *) thread, arg);
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
		pthread_join(curr->thread, NULL);
		curr = curr->next;
	}
	pthread_join(monitor_tread, NULL);
	pthread_mutex_destroy(&arg->config->mutex_console);
	pthread_mutex_destroy(&arg->config->mutex_burn);
	free(arg->config);
	free_coders(arg->coder);
	free_dongles(arg->dongle);
}

int	main(int argc, char **argv)
{
	long long	start;
	t_config	*config;
	pthread_t	monitor_tread;
	t_arg		arg;

	start = ft_get_time();
	config = parse(argc, argv);
	if (!config)
	{
		printf("Error");
		return (0);
	}
	arg.config = config;
	arg.start = start;
	arg.coder = init_coders(config->number_coders);
	arg.dongle = init_dongles(config->number_coders);
	pthread_create(&monitor_tread, NULL, (void *) monitor, &arg);
	if (!execute_all(arg.coder, arg.dongle, start, config))
		printf("Error\n");
	end(&arg, monitor_tread);
	return (0);
}
