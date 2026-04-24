/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sloubiat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 00:29:01 by sloubiat          #+#    #+#             */
/*   Updated: 2026/04/20 18:46:14 by sloubiat         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */
#include "header/codexion.h"

t_coders	*init_coders(int nbr)
{
	t_coders	*coders;
	t_coders	*actual;
	int			id;

	id = 1;
	coders = malloc(sizeof(t_coders));
	if (!set_coder_default(coders, &id))
	{
		if (coders)
			free_coders(coders);
		return (0);
	}
	actual = coders;
	while (nbr--)
	{
		if (!init_data_coder(coders, actual, &id))
			return (0);
		actual = actual->next;
	}
	actual->next = coders;
	coders->prev = actual;
	return (coders);
}

t_dongle	*init_dongles(int nbr)
{
	t_dongle	*dongles;
	t_dongle	*actual;
	int			id;

	id = 1;
	dongles = malloc(sizeof(t_dongle));
	if (!set_dongle_default(dongles, &id))
	{
		if (dongles)
			free_dongles(dongles);
		return (0);
	}
	actual = dongles;
	while (nbr--)
	{
		if (!init_data_dongle(dongles, actual, &id))
			return (0);
		actual = actual->next;
	}
	actual->next = dongles;
	dongles->prev = actual;
	return (dongles);
}

int	init_config(t_config *config, char **argv)
{
	config->number_coders = ft_atoi(argv[1]);
	config->time_burnout = ft_atoi(argv[2]);
	config->time_compile = ft_atoi(argv[3]);
	config->time_debug = ft_atoi(argv[4]);
	config->time_refactor = ft_atoi(argv[5]);
	config->number_compile = ft_atoi(argv[6]);
	config->dongle_cooldown = ft_atoi(argv[7]);
	config->burned = 0;
	config->end = 0;
	if (pthread_mutex_init(&config->mutex_console, NULL) != 0)
	{
		free_config(config);
		return (0);
	}
	if (pthread_mutex_init(&config->mutex_burn, NULL) != 0)
	{
		pthread_mutex_destroy(&config->mutex_console);
		free(config);
		return (0);
	}
	if (ft_strcmp(argv[8], "edf") == 0)
		config->edf = 1;
	else
		config->edf = 0;
	return (1);
}
