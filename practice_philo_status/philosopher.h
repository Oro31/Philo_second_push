/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvalton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 18:55:32 by rvalton           #+#    #+#             */
/*   Updated: 2021/10/28 14:55:52 by rvalton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H
# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>

# define psleep(sec) sleep ((sec))
# define INITIAL_STOCK	20
# define NB_CLIENTS		5

typedef struct	s_philo {
	pthread_t		th_philo;
	struct timeval	last_meal;
	struct timeval	start;
	struct timeval	t;
	int				alive;
	int				id;
	int				eat;
	int				thi;
	int				sle;
}				t_philo;

typedef struct	s_fork {
	pthread_mutex_t	mutex;
	int				id;
	int				use;
}				t_fork;

typedef struct	s_vars {
	t_philo			*philos;
	t_fork			*forks;
	pthread_mutex_t	mutex;
	int				id_philo;
	int				nb_philos;
	int				ttd;
	int				tte;
	int				tts;
	int				nb_must_eat;
}				t_vars;

typedef struct	s_cient {
	pthread_t		th_client;
	pthread_mutex_t	mutex;
	int				id;
	int				ordering;
}				t_client;

typedef struct	s_store {
	int			stock;
	pthread_mutex_t	mutex;
	pthread_t	th_store;
	t_client	clients [NB_CLIENTS];
}				t_store;

static t_store	store = 
{
	.stock = INITIAL_STOCK,
};

#endif
