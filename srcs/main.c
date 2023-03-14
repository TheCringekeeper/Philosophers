/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankhabar <ankhabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 15:46:31 by ankhabar          #+#    #+#             */
/*   Updated: 2023/03/14 20:11:52 by ankhabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

u_int64_t	get_time(void)
{
	struct timeval	tp;
	u_int64_t		milliseconds;

	gettimeofday(&tp, NULL);
	milliseconds = tp.tv_sec * 1000;
	milliseconds += tp.tv_usec / 1000;
	return (milliseconds);
}

u_int64_t	timestamp(t_data *data)
{
	return (get_time() - data->start_time);
}

void	*killer(void *data)
{
	t_philo	*philo;

	philo = data;
	while (1)
	{
		if ((get_time() - philo->data->start_time) > (unsigned long int)philo->data->time_to_die)
			break;
	}
	philo->dead = true;
	return (0);
}

void	excluded_printf(t_philo *philo, char *code)
{
	pthread_mutex_lock(&philo->data->mutexes[PRINT]);
	printf("%lums %i %s\n", timestamp(philo->data), philo->id, code);
	pthread_mutex_unlock(&philo->data->mutexes[PRINT]);
}

void	*ft_philo(void *data)
{
	t_philo	*philo;

	philo = data;
	if ((philo->id % 2) == 0)
		usleep(1000);
	while (1)
	{
		excluded_printf(philo, THINK);
		pthread_mutex_lock(&philo->forks[philo->left_fork]);
		excluded_printf(philo, FORK);
		pthread_mutex_lock(&philo->forks[philo->right_fork]);
		excluded_printf(philo, FORK);
		excluded_printf(philo, EAT);
		usleep(philo->data->time_to_eat);
		pthread_mutex_unlock(&philo->forks[philo->left_fork]);
		pthread_mutex_unlock(&philo->forks[philo->right_fork]);
		excluded_printf(philo, SLEEP);
		usleep(philo->data->time_to_sleep);
	}
	return (0);
}

bool	input_error(t_data *data)
{
	return (data->start_time < 0 || data->philosophers < 0
		|| data->time_to_die < 0 || data->time_to_eat < 0
		|| data->time_to_die < 0 || data->mutexes == NULL);
}

t_mutex	*init_mutexes(void)
{
	int		index;
	t_mutex *mutexes;

	index = 0;
	mutexes = NULL;
	mutexes = malloc(sizeof(t_mutex) * M_NUM);
	if (mutexes == NULL)
		return (NULL);
	while (index < M_NUM)
		pthread_mutex_init(&mutexes[index++], 0);
	return (mutexes);
}

t_data	*input_scanner(int ac, char *av[])
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (data == NULL)
		return (NULL);
	data->start_time = -1;
	data->philosophers = -1;
	data->time_to_die = -1;
	data->time_to_eat = -1;
	data->time_to_sleep = -1;
	data->must_eat = -1;
	data->mutexes = NULL;
	data->start_time = get_time();
	data->philosophers = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]) * 1000;
	data->time_to_eat = ft_atoi(av[3]) * 1000;
	data->time_to_sleep = ft_atoi(av[4]) * 1000;
	if (ac == 6)
		data->must_eat = ft_atoi(av[5]);
	data->mutexes = init_mutexes();
	printf("data initalized: %lu, %i, %i, %i, %i\n", data->start_time, data->philosophers, data->time_to_die, data->time_to_eat, data->time_to_sleep);
	if (input_error(data))
		return (NULL);
	return (data);
}

bool	init_philos(t_philo *philos, t_data *data)
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
		philos[i].id = i;
		philos[i].left_fork = i;
		philos[i].right_fork = (i + 1);
		philos[i].dead = false;
		philos[i].forks = fork;
		philos[i].data = data;
		printf("philo number %i is ready to work\n", i);
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

// add error case
void	init_pthreads(t_philo *philos)
{
	int			index;
	pthread_t	*id;
	pthread_t	keanu;

	index = 0;
	id = malloc(sizeof(pthread_t) * philos->data->philosophers);
	if (id == NULL)
		return ;
	while (index < philos->data->philosophers)
	{
		pthread_create(&id[index], 0, ft_philo, (void *)&philos[index]);
		index++;
	}
	pthread_create(&keanu, 0, killer, (void *)&philos[0]);
	while (philos[0].dead != true)
		continue ;
	while (index < philos->data->philosophers)
		pthread_join(id[index++], 0);
}

int	main(int ac, char *av[])
{
	t_philo	*philos;

	if (ac > 4 && ac < 7)
	{
		philos = init_struct(ac, av);
		init_pthreads(philos);
	}
	return (0);
}
