/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_dongle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sloubiat <sloubiat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 18:14:31 by sloubiat          #+#    #+#             */
/*   Updated: 2026/04/22 15:13:01 by sloubiat         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "header/codexion.h"

int	set_coder_default(t_coders *coder, int *id)
{
	if (!coder)
		return (0);
	coder->id = (*id)++;
	coder->number_compile = 0;
	coder->last_use = 0;
	coder->prev = 0;
	coder->next = 0;
	coder->thread = 0;
	if (pthread_mutex_init(&coder->coder_mutex, NULL))
		return (0);
	return (1);
}

int	init_data_coder(t_coders *coders, t_coders *actual, int *id)
{
	actual->next = malloc(sizeof(t_coders));
	if (!set_coder_default(actual->next, id))
	{
		free_coders(coders);
		return (0);
	}
	actual->next->prev = actual;
	return (1);
}
