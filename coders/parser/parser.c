/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sloubiat <sloubiat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 15:47:43 by sloubiat          #+#    #+#             */
/*   Updated: 2026/04/22 00:19:44 by sloubiat         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"

static int	check_nb(char *nbr)
{
	int	nb;

	nb = ft_atoi(nbr);
	if (nb == 0)
		if (ft_strcmp(nbr, "0") && ft_strcmp(nbr, "-0") && ft_strcmp(nbr, "+0"))
			return (0);
	if (nb < 0)
		return (0);
	return (1);
}

void	*show_help(void)
{
	printf("./codexion <coders> <time_to_burnout> ");
	printf("<time_to_compile> <time_to_debug> <time_to_refactor> ");
	printf("<nb_compiles_required> <dongle_cooldown> <scheduler>\n");
	return (0);
}

void	*free_conf(t_config *config)
{
	free(config);
	return (0);
}

t_config	*parse(int argc, char **argv)
{
	t_config	*config;

	if (argc < 9)
		return (show_help());
	config = (t_config *) malloc(sizeof(t_config));
	if (!config)
		return (0);
	if (!check_nb(argv[1]) || !check_nb(argv[2])
		|| !check_nb(argv[3]) || !check_nb(argv[4])
		|| !check_nb(argv[5]) || !check_nb(argv[6]) || !check_nb(argv[7])
		|| atoi(argv[1]) == 0)
		return (free_conf(config));
	if (ft_strcmp(argv[8], "fifo") != 0 && ft_strcmp(argv[8], "edf") != 0)
		return (free_conf(config));
	if (!init_config(config, argv))
		return (free_conf(config));
	return (config);
}
