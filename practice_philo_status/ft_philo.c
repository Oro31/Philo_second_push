/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvalton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 12:54:42 by rvalton           #+#    #+#             */
/*   Updated: 2021/10/28 15:01:23 by rvalton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static int	ft_dead(t_vars *vars)
{
	int	i;

	i = -1;
	while (++i < vars->nb_philos)
	{
		if (vars->philos[i].alive == 0)
			return (1);
	}
	return (0);
}

static void	*fn_philo(void *p_data)
{
	t_vars	*vars;
	int		i;
	int		ret;

	vars = (t_vars *)p_data;
	i = vars->id_philo;
	vars->philos[i].thi = 1;
	printf("philo %d thinking\n", i);
	while (!ft_dead(vars))
	{
		/*printf("philo %d : alive = %d, eat = %d, thi = %d, sle = %d\n", i, vars->philos[i].alive,
				vars->philos[i].eat, vars->philos[i].thi, vars->philos[i].sle);
	*/	if (vars->forks[i].use == 0 && vars->forks[i + 1].use == 0 &&
				vars->philos[i].thi == 1)
		{
			pthread_mutex_lock(&vars->forks[i].mutex);
			pthread_mutex_lock(&vars->forks[i + 1].mutex);
			vars->forks[i].use = 1;
			vars->forks[(i + 1) % vars->nb_philos].use = 1;
			pthread_mutex_lock(&vars->mutex);
			ret = gettimeofday(&vars->philos[i].start, NULL);
			ret = gettimeofday(&vars->philos[i].t, NULL);
			printf("%d Philosopher %d has taken a fork\n", (((int)vars->philos[i].t.tv_sec) * 1000000) +
					vars->philos[i].t.tv_usec, i);
			printf("%d Philosopher %d is eating\n", (((int)vars->philos[i].t.tv_sec) * 1000000) +
					vars->philos[i].t.tv_usec, i);
			pthread_mutex_unlock(&vars->mutex);
			vars->philos[i].thi = 0;
			vars->philos[i].eat = 1;
			while ((vars->philos[i].t.tv_sec * 1000000) + vars->philos[i].t.tv_usec <
					(vars->philos[i].start.tv_sec * 1000000) + vars->philos[i].start.tv_sec + vars->tte)
				ret = gettimeofday(&vars->philos[i].t, NULL);
			vars->forks[i].use = 0;
			vars->forks[(i + 1) % vars->nb_philos].use = 0;
			pthread_mutex_unlock(&vars->forks[i].mutex);
			pthread_mutex_unlock(&vars->forks[i + 1].mutex);
		}
		if (vars->philos[i].eat == 1)
		{
			ret = gettimeofday(&vars->philos[i].last_meal, NULL);
			ret = gettimeofday(&vars->philos[i].t, NULL);
			pthread_mutex_lock(&vars->mutex);
			printf("%d Philosopher %d is sleeping\n", (((int)vars->philos[i].t.tv_sec) * 1000000) +
					vars->philos[i].t.tv_usec, i);
			pthread_mutex_unlock(&vars->mutex);
			vars->philos[i].eat = 0;
			vars->philos[i].sle = 1;
			while ((vars->philos[i].t.tv_sec * 1000000) + vars->philos[i].t.tv_usec <
					(vars->philos[i].last_meal.tv_sec * 1000000) + vars->philos[i].last_meal.tv_sec + vars->tts &&
					vars->philos[i].alive)
			{
				ret = gettimeofday(&vars->philos[i].t, NULL);
				if ((vars->philos[i].t.tv_sec * 1000000) + vars->philos[i].t.tv_usec >=
						(vars->philos[i].last_meal.tv_sec * 1000000) + vars->philos[i].last_meal.tv_sec + vars->ttd)
					vars->philos[i].alive = 0;
			}
		}
		if (vars->philos[i].sle == 1 && vars->philos[i].alive)
		{
			pthread_mutex_lock(&vars->mutex);
			printf("%d Philosopher %d is thinking\n", (((int)vars->philos[i].t.tv_sec) * 1000000) +
					vars->philos[i].t.tv_usec, i);
			pthread_mutex_unlock(&vars->mutex);
			vars->philos[i].sle = 0;
			vars->philos[i].thi = 1;
		}
		ret = gettimeofday(&vars->philos[i].t, NULL);
		if ((vars->philos[i].t.tv_sec * 1000000) + vars->philos[i].t.tv_usec <
				(vars->philos[i].last_meal.tv_sec * 1000000) + vars->philos[i].last_meal.tv_sec + vars->ttd)
			vars->philos[i].alive = 0;
	}
	if (vars->philos[i].alive == 0)
	{
		ret = gettimeofday(&vars->philos[i].t, NULL);
		pthread_mutex_lock(&vars->mutex);
		printf("%d Philosopher %d is dead\n", (((int)vars->philos[i].t.tv_sec) * 1000000) +
				vars->philos[i].t.tv_usec, i);
		pthread_mutex_unlock(&vars->mutex);
	}
}

int	main(int argc, char **argv)
{
	t_vars	all;
	int		ret;
	int		i;

	if (argc < 5)
		return (0);
	all.nb_philos = atoi(argv[1]);
	all.ttd = atoi(argv[2]);
	all.tte = atoi(argv[3]);
	all.tts = atoi(argv[4]);
	if (argc == 6)
		all.nb_must_eat = atoi(argv[5]);
	all.philos = malloc(sizeof(t_philo) * all.nb_philos);
	all.forks = malloc(sizeof(t_fork) * all.nb_philos);
	ret = pthread_mutex_init(&all.mutex, NULL);
	if (ret)
		printf("Error\n");
	i = -1;
	printf("create philos thread, nb = %d\n", all.nb_philos);
	while (++i < all.nb_philos)
	{
		all.philos[i].id = i;
		all.philos[i].alive = 1;
		all.forks[i].id = i;
		all.forks[i].use = 0;
		ret = pthread_mutex_init(&all.forks[i].mutex, NULL);
		if (ret)
			printf("Error\n");
		all.id_philo = i;
		printf("philo %d created\n", i);
		ret = pthread_create(&all.philos[i].th_philo, NULL, fn_philo, &all);
		if (ret)
			printf("Error\n");
	}
	i = -1;
	while (++i < all.nb_philos)
	{
		all.id_philo = i;
		printf("philo %d join\n", i);
		pthread_join(all.philos[i].th_philo, NULL);
	}
	return (0);
}
