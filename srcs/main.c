/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankhabar <ankhabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 15:46:31 by ankhabar          #+#    #+#             */
/*   Updated: 2023/03/19 22:23:20 by ankhabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*ft_philo(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if ((philo->id % 2) == 0)
	{
		excluded_printf(philo, THINK);
		usleep(1000);
	}
	while (philo->data->someone_dead == false)
	{
		pthread_mutex_lock(&philo->forks[philo->left_fork]);
		excluded_printf(philo, FORK);
		pthread_mutex_lock(&philo->forks[philo->right_fork]);
		excluded_printf(philo, FORK);
		excluded_printf(philo, EAT);
		// ft_usleep(philo->data->time_to_eat);
		smart_sleep(philo->data->time_to_eat, philo);
		pthread_mutex_unlock(&philo->forks[philo->left_fork]);
		pthread_mutex_unlock(&philo->forks[philo->right_fork]);
		pthread_mutex_lock(&philo->data->mutexes[TIME]);
		philo->last_eat = get_time();
		pthread_mutex_unlock(&philo->data->mutexes[TIME]);
		excluded_printf(philo, SLEEP);
		// ft_usleep(philo->data->time_to_sleep);
		smart_sleep(philo->data->time_to_sleep, philo);
		excluded_printf(philo, THINK);
	}
	return (0);
}

// tut mutex nuzhen, eto tipa killer tred
void	finish_simulation(t_philo *philos)
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

void	killer(t_philo *philos)
{
	bool	stay;
	int		i;

	i = 0;
	stay = true;
	usleep(1000);
	while (stay)
	{
		i = 0;
		while (i < philos->data->philosophers)
		{
			pthread_mutex_lock(&philos[i].data->mutexes[TIME]);
			if ((get_time() - philos[i].last_eat) >= (unsigned long int)philos[i].data->time_to_die)
			{
				pthread_mutex_unlock(&philos[i].data->mutexes[TIME]);
				pthread_mutex_lock(&philos[i].data->mutexes[DEAD]);
				philos->data->someone_dead = true;
				pthread_mutex_unlock(&philos[i].data->mutexes[DEAD]);
				stay = false;
				final_print(&philos[i]);
				break ;
			}
			pthread_mutex_unlock(&philos[i].data->mutexes[TIME]);
			i++;
		}
	}
}

// add error cases
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
	killer(philos);
	finish_simulation(philos);
	while (index < philos->data->philosophers)
		pthread_join(id[index++], 0);
	free(id);
}

// ./philo 5 202 100 100 sometimes doesnt work, 201 never works
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
