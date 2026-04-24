/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sloubiat <sloubiat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 02:25:01 by sloubiat          #+#    #+#             */
/*   Updated: 2026/04/18 02:28:45 by sloubiat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header/codexion.h"

void	free_coders(t_coders *coders)
{
	t_coders	*temp;

	if (coders && coders->prev)
		coders->prev->next = NULL;
	while (coders)
	{
		temp = coders->next;
		pthread_mutex_destroy(&coders->coder_mutex);
		free(coders);
		coders = temp;
	}
}

void	free_dongles(t_dongle *dongles)
{
	t_dongle	*temp;

	if (dongles && dongles->prev)
		dongles->prev->next = NULL;
	while (dongles)
	{
		temp = dongles->next;
		pthread_mutex_destroy(&dongles->mutex);
		pthread_mutex_destroy(&dongles->queue_mutex);
		free(dongles);
		dongles = temp;
	}
}

void	free_config(t_config *config)
{
	pthread_mutex_destroy(&config->mutex_burn);
	pthread_mutex_destroy(&config->mutex_console);
	free(config);
}

void	free_all(t_config *config, t_dongle *dongles, t_coders *coders)
{
	free_config(config);
	free_dongles(dongles);
	free_coders(coders);
}
