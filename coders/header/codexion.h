/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sloubiat <sloubiat@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 16:49:13 by sloubiat          #+#    #+#             */
/*   Updated: 2026/04/20 18:45:57 by sloubiat         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <time.h>
# include <unistd.h>

typedef struct s_config
{
	int				number_coders;
	int				time_burnout;
	int				time_compile;
	int				time_debug;
	int				time_refactor;
	int				number_compile;
	int				dongle_cooldown;
	int				burned;
	int				edf;
	int				end;
	pthread_mutex_t	mutex_console;
	pthread_mutex_t	mutex_burn;
}	t_config;

typedef struct s_coders
{
	int				id;
	int				number_compile;
	int				last_use;
	pthread_t		thread;
	pthread_mutex_t	coder_mutex;
	struct s_coders	*next;
	struct s_coders	*prev;
}	t_coders;

typedef struct s_dongle
{
	int				id;
	int				last_use;
	int				queue[2];
	pthread_mutex_t	queue_mutex;
	pthread_mutex_t	mutex;
	struct s_dongle	*next;
	struct s_dongle	*prev;
}	t_dongle;

typedef struct s_arg
{
	t_coders	*coder;
	t_dongle	*dongle;
	long long	start;
	t_config	*config;
}	t_arg;

// parser
t_config	*parse(int argc, char **argv);

// parser_utils
int			ft_atoi(const char *s);
int			ft_strcmp(const char *s1, const char *s2);

// init
t_coders	*init_coders(int nbr);
t_dongle	*init_dongles(int nbr);
void		init_config(t_config *config, char **argv);

// mutex_utils
void		register_mutex(t_dongle *dongle, t_coders *coder);
int			mutex_lock(t_dongle *dongle, t_arg *arg, t_coders *coder);
void		mutex_unlock(t_dongle *dongle, t_arg *arg);

// cleanup
void		free_coders(t_coders *coders);
void		free_dongles(t_dongle *dongles);

// utils
void		ft_put_str(char *str, int coder_id, t_arg *arg);
int			has_finish(t_arg *arg, t_coders *coder);
int			ft_get_time(void);
int			has_burned(t_arg *arg);

// monitor
void		monitor(t_arg *arg);

// monitor utils
void		swap(int *queue);
void		lock_mutex(t_coders *coders);
void		unlock_mutex(t_coders *coders);
void		set_burnout(t_coders *coders, t_arg *arg);

// thread
void		thread(t_arg *arg);
//main
int			ft_get_time(void);

#endif
