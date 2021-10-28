/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvalton <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 19:06:19 by rvalton           #+#    #+#             */
/*   Updated: 2021/10/27 17:56:52 by rvalton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static int	get_rnd(int max)
{
	double	val;
	int		ret;

	val = (double)(max * rand());
	ret = ((int)val % 19) + 1;
	if (ret < 0)
		ret = ret * -1;
	return (ret);
}

static void	*fn_store(void *p_data)
{
	int	i;

	while (1 && p_data == NULL)
	{
		if (store.stock == 0)
		{
			pthread_mutex_lock(&store.mutex);
			store.stock = INITIAL_STOCK;
			printf("Remplissage du stock !\n");
			i = -1;
			while (++i < NB_CLIENTS)
			{
				if (store.clients[i].ordering == 1)
					pthread_mutex_unlock(&store.clients[i].mutex);
			}
			pthread_mutex_unlock(&store.mutex);
		}
	}
	return (NULL);
}

static void	*fn_clients(void *p_data)
{
	t_client	*client;
	int			val;

	client = (t_client *)p_data;
/*	pthread_mutex_t	*mutex;

	mutex = (pthread_mutex_t *)p_data;*/
	client->ordering = 0;
	val = 1;
	pthread_mutex_lock(&client->mutex);
	while (1)
	{
		if (client->ordering == 0)
			pthread_mutex_unlock(&client->mutex);
		pthread_mutex_lock(&client->mutex);
		if (client->ordering == 0)
		{
			psleep(get_rnd(3));
			val = get_rnd(6);
			printf("Client %d want %d articles\n", client->id, val);
		}
		if (store.stock - val < 0)
		{
			pthread_mutex_lock(&store.mutex);
			client->ordering = 1;
			store.stock = 0;
			printf("Stock need refiel\n");
			pthread_mutex_unlock(&store.mutex);
		}
		if (store.stock - val >= 0)
		{
			pthread_mutex_lock(&store.mutex);
			client->ordering = 0;
			store.stock = store.stock - val;
			printf("Client %d prend %d du stock, reste %d en stock !\n", client->id, val, store.stock);
			pthread_mutex_unlock(&store.mutex);
			/*pthread_mutex_unlock(&client->mutex);*/
		}
	}
	return (NULL);
}

int	main(void)
{
	int	i;
	int	ret;

	ret = pthread_mutex_init(&store.mutex, NULL);
	if (ret)
		printf("error : %d\n", ret);
	i = -1;
	while (++i < NB_CLIENTS)
	{
		ret = pthread_mutex_init(&store.clients[i].mutex, NULL);
		if (ret)
			printf("error : %d\n", ret);
	}
	i = 0;
	ret = 0;
	printf("Creation du thread du magasin !\n");
	ret = pthread_create(&store.th_store, NULL, fn_store, NULL);
	if (!ret)
	{
		printf("Creation des threads clients !\n");
		while (i < NB_CLIENTS)
		{
			store.clients[i].id = i;
			ret = pthread_create(&store.clients[i].th_client, NULL, fn_clients, &store.clients[i]);
			if (ret)
				printf("error : %d\n", ret);
			i++;
		}
	}
	else
		printf("error : %d\n", ret);
	i = 0;
	while (i < NB_CLIENTS)
	{
		pthread_join(store.clients[i].th_client, NULL);
		i++;
	}
	pthread_join(store.th_store, NULL);
/*	pthread_mutex_destroy(&mutex);*/
	return (0);
}
