/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankhabar <ankhabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 20:55:05 by ankhabar          #+#    #+#             */
/*   Updated: 2023/03/28 09:30:15 by ankhabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	input_error(t_data *data, int ac)
{
	return (data->philosophers <= 0 || (ac == 6 && data->must_eat < 0)
		|| data->time_to_die <= 0 || data->time_to_eat <= 0
		|| data->sim_start == 0
		|| data->time_to_sleep <= 0 || data->mutexes == NULL);
}

static t_data	*input_scanner(int ac, char *av[])
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (data == NULL)
		return (NULL);
	data->must_eat = -1;
	data->mutexes = NULL;
	data->should_exit = false;
	data->sim_start = get_time();
	data->philosophers = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		data->must_eat = ft_atoi(av[5]);
	data->mutexes = init_mutexes();
	if (input_error(data, ac))
		return (free(data->mutexes), free(data), NULL);
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
	t_mutex	m_eat;
	t_mutex	m_last;

	i = 0;
	fork = malloc(sizeof(t_mutex) * data->philosophers);
	if (fork == NULL)
		return (EXIT_FAILURE);
	while (i < data->philosophers)
		pthread_mutex_init(&fork[i++], 0);
	pthread_mutex_init(&m_eat, 0);
	pthread_mutex_init(&m_last, 0);
	i = 0;
	while (i < data->philosophers)
	{
		philos[i].last_eat = get_time();
		philos[i].id = i + 1;
		philos[i].eat_times = 0;
		if (data->must_eat == -1)
			philos[i].eat_times--;
		philos[i].left_fork = i;
		philos[i].right_fork = (i + 1);
		if (i == (data->philosophers) - 1)
			philos[i].right_fork = 0;
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
	if (data == NULL)
		return (NULL);
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
