/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankhabar <ankhabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 15:46:31 by ankhabar          #+#    #+#             */
/*   Updated: 2023/03/18 20:47:46 by ankhabar         ###   ########.fr       */
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

u_int64_t	timestamp(t_philo *philo, u_int64_t sim_start)
{
	u_int64_t	test;

	pthread_mutex_lock(&philo->data->mutexes[TIME]);
	test = get_time() - sim_start;
	pthread_mutex_unlock(&philo->data->mutexes[TIME]);
	return (test);
}

void	excluded_printf(t_philo *philo, char *code)
{
	pthread_mutex_lock(&philo->data->mutexes[DEAD]);
	if (philo->dead == false)
	{
		pthread_mutex_lock(&philo->data->mutexes[PRINT]);
		printf("%lu %i %s\n", timestamp(philo, philo->data->sim_start), philo->id, code);
		pthread_mutex_unlock(&philo->data->mutexes[PRINT]);
	}
	pthread_mutex_unlock(&philo->data->mutexes[DEAD]);
}

void	ft_usleep(int to_sleep)
{
	usleep(to_sleep * 1000);
}

void	*ft_philo(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if ((philo->id % 2) == 0)
	{
		excluded_printf(philo, THINK);
		usleep(1000);
	}
	while (1)
	{
		pthread_mutex_lock(&philo->forks[philo->left_fork]);
		excluded_printf(philo, FORK);
		pthread_mutex_lock(&philo->forks[philo->right_fork]);
		excluded_printf(philo, FORK);
		excluded_printf(philo, EAT);
		ft_usleep(philo->data->time_to_eat);
		pthread_mutex_unlock(&philo->forks[philo->left_fork]);
		pthread_mutex_unlock(&philo->forks[philo->right_fork]);
		pthread_mutex_lock(&philo->data->mutexes[TIME]);
		philo->last_eat = get_time();
		pthread_mutex_unlock(&philo->data->mutexes[TIME]);
		excluded_printf(philo, SLEEP);
		ft_usleep(philo->data->time_to_sleep);
		excluded_printf(philo, THINK);
	}
	return (0);
}

bool	input_error(t_data *data)
{
	return (data->sim_start < 0 || data->philosophers < 0
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
	data->sim_start = -1;
	data->philosophers = -1;
	data->time_to_die = -1;
	data->time_to_eat = -1;
	data->time_to_sleep = -1;
	data->must_eat = -1;
	data->mutexes = NULL;
	data->sim_start = get_time();
	data->philosophers = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		data->must_eat = ft_atoi(av[5]);
	data->mutexes = init_mutexes();
	printf("data initalized: %lu, %i, %i, %i, %i\n", data->sim_start, data->philosophers, data->time_to_die, data->time_to_eat, data->time_to_sleep);
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
		philos[i].last_eat = get_time();
		philos[i].id = i;
		philos[i].left_fork = i;
		philos[i].right_fork = (i - 1);
		if (i == 0)
			philos[i].right_fork = (data->philosophers);
		philos[i].dead = false;
		philos[i].forks = fork;
		philos[i].data = data;
		printf("philo %i ready to work\n", i);
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

// tut mutex nuzhen, eto tipa killer tred
void finish_simulation(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < (philos[0].data->philosophers))
	{
		pthread_mutex_lock(&philos[i].data->mutexes[DEAD]);
		philos[i].dead = true;
		pthread_mutex_unlock(&philos[i++].data->mutexes[DEAD]);
	}
}

void	set_counters(t_philo *philos, t_data *data)
{
	int	i;

	pthread_mutex_lock(&data->mutexes[SYNK]);
	data->sim_start = get_time();
	i = 0;
	while (i < philos->data->philosophers)
	{
		pthread_mutex_lock(&data->mutexes[EATS]);
		philos[i].last_eat = data->sim_start;
		pthread_mutex_unlock(&data->mutexes[EATS]);
		i++;
	}
	pthread_mutex_unlock(&data->mutexes[SYNK]);
}

// add error case
void	init_pthreads(t_philo *philos)
{
	int			index;
	pthread_t	*id;

	index = 0;
	id = malloc(sizeof(pthread_t) * philos->data->philosophers);
	if (id == NULL)
		return ;
	while (index < philos->data->philosophers)
	{
		pthread_create(&id[index], 0, ft_philo, (void *)&philos[index]);
		index++;
	}
	set_counters(philos, philos->data);
	int stay = 1;
	int	i = 0;
	usleep(1000);
	while (stay)
	{
		i = 0;
		while (i < philos->data->philosophers)
		{
			if ((get_time() - philos[i].last_eat) >= (unsigned long int)philos[i].data->time_to_die)
			{
				excluded_printf(&philos[i], DIED);
				stay = 0;
				break ;
			}
			i++;
		}
	}
	finish_simulation(philos);
	while (index < philos->data->philosophers)
		pthread_join(id[index++], 0);
	free(id);
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
