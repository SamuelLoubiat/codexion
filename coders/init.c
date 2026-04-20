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

void	set_coder_default(t_coders *coder, int *id)
{
	coder->id = (*id)++;
	coder->number_compile = 0;
	coder->last_use = 0;
	pthread_mutex_init(&coder->coder_mutex, NULL);
}

t_coders	*init_coders(int nbr)
{
	t_coders	*coders;
	t_coders	*actual;
	t_coders	*last;
	int			id;

	id = 1;
	coders = malloc(sizeof(t_coders));
	set_coder_default(coders, &id);
	actual = coders;
	nbr--;
	while (nbr--)
	{
		actual->next = malloc(sizeof(t_coders));
		last = actual;
		actual = actual->next;
		set_coder_default(actual, &id);
		actual->prev = last;
	}
	actual->next = coders;
	coders->prev = actual;
	return (coders);
}

void	set_dongle_default(t_dongle *dongle, int *id)
{
	dongle->id = (*id)++;
	dongle->last_use = 0;
	dongle->queue[0] = 0;
	dongle->queue[1] = 0;
	pthread_mutex_init(&dongle->mutex, NULL);
	pthread_mutex_init(&dongle->queue_mutex, NULL);
}

t_dongle	*init_dongles(int nbr)
{
	t_dongle	*dongles;
	t_dongle	*actual;
	int			id;

	id = 1;
	dongles = malloc(sizeof(t_dongle));
	set_dongle_default(dongles, &id);
	actual = dongles;
	nbr--;
	while (nbr--)
	{
		actual->next = malloc(sizeof(t_dongle));
		actual->next->prev = actual;
		actual = actual->next;
		set_dongle_default(actual, &id);
	}
	actual->next = dongles;
	dongles->prev = actual;
	return (dongles);
}

void	init_config(t_config *config, char **argv)
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
	pthread_mutex_init(&config->mutex_console, NULL);
	pthread_mutex_init(&config->mutex_burn, NULL);
	if (ft_strcmp(argv[8], "edf") == 0)
		config->edf = 1;
	else
		config->edf = 0;
}
