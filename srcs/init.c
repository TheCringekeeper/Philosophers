/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 20:55:05 by ankhabar          #+#    #+#             */
/*   Updated: 2023/03/25 01:42:53 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	input_error(t_data *data)
{
	return (data->philosophers < 0
		|| data->time_to_die < 0 || data->time_to_eat < 0
		|| data->time_to_die < 0 || data->mutexes == NULL);
}

static t_data	*input_scanner(int ac, char *av[])
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (data == NULL)
		return (NULL);
	data->sim_start = -1;
	data->philosophers = -1;
	data->time_to_die = -1;
	data->time_to_eat = -1;
	data->time_to_sleep = -1;
	data->must_eat = -1;
	data->mutexes = NULL;
	data->someone_dead = false;
	data->sim_start = get_time();
	data->philosophers = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		data->must_eat = ft_atoi(av[5]);
	data->mutexes = init_mutexes();
	if (input_error(data))
		return (NULL);
	return (data);
}

t_mutex	*init_mutexes(void)
{
	int		index;
	t_mutex	*mutexes;

	index = 0;
	mutexes = NULL;
	mutexes = malloc(sizeof(t_mutex) * M_NUM);
	if (mutexes == NULL)
		return (NULL);
	while (index < M_NUM)
		pthread_mutex_init(&mutexes[index++], 0);
	return (mutexes);
}

static bool	init_philos(t_philo *philos, t_data *data)
{
	int		i;
	t_mutex	*fork;

	i = 0;
	fork = malloc(sizeof(t_mutex) * data->philosophers);
	if (fork == NULL)
		return (EXIT_FAILURE);
	while (i < data->philosophers)
		pthread_mutex_init(&fork[i++], 0);
	i = 0;
	while (i < data->philosophers)
	{
		philos[i].last_eat = get_time();
		philos[i].id = i;
		philos[i].left_fork = i;
		philos[i].right_fork = (i + 1);
		if (i == (data->philosophers) - 1)
			philos[i].right_fork = 0;
		philos[i].dead = false;
		philos[i].forks = fork;
		philos[i].data = data;
		i++;
	}
	return (EXIT_SUCCESS);
}

t_philo	*init_struct(int ac, char *av[])
{
	t_philo	*philos;
	t_data	*data;

	data = input_scanner(ac, av);
	philos = malloc(sizeof(t_philo) * data->philosophers);
	if (philos == NULL)
		return (NULL);
	philos->data = data;
	if (philos->data == NULL)
		return (NULL);
	if (init_philos(philos, philos->data) == EXIT_FAILURE)
		return (NULL);
	return (philos);
}
