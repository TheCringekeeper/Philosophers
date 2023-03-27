/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankhabar <ankhabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 09:26:22 by ankhabar          #+#    #+#             */
/*   Updated: 2023/03/27 10:00:55 by ankhabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	fork_release(struct s_philo *philo, int forks)
{
	if (death_check(philo) == true)
	{
		if (philo->left_fork < philo->right_fork)
		{
			pthread_mutex_unlock(&philo->forks[philo->left_fork]);
			if (forks == 2)
				pthread_mutex_unlock(&philo->forks[philo->right_fork]);
			return (true);
		}
		else
		{
			pthread_mutex_unlock(&philo->forks[philo->right_fork]);
			if (forks == 2)
				pthread_mutex_unlock(&philo->forks[philo->left_fork]);
			return (true);
		}
	}
	return (false);
}

bool	ft_eating(t_philo *philo)
{
	excluded_printf(philo, FORK);
	excluded_printf(philo, EAT);
	pthread_mutex_lock(&philo->data->mutexes[EATS]);
	if (philo->eat_times != -1)
		philo->eat_times--;
	pthread_mutex_unlock(&philo->data->mutexes[EATS]);
	smart_sleep(philo->data->time_to_eat, philo);
	pthread_mutex_unlock(&philo->forks[philo->left_fork]);
	pthread_mutex_unlock(&philo->forks[philo->right_fork]);
	pthread_mutex_lock(&philo->data->mutexes[TIME]);
	philo->last_eat = get_time();
	pthread_mutex_unlock(&philo->data->mutexes[TIME]);
	return (true);
}

void	choosing_fork(t_philo *philo, int order)
{
	if (order == FIRST)
	{
		if (philo->left_fork < philo->right_fork)
			pthread_mutex_lock(&philo->forks[philo->left_fork]);
		else
			pthread_mutex_lock(&philo->forks[philo->right_fork]);
	}
	else if (order == SECOND)
	{
		if (philo->left_fork < philo->right_fork)
			pthread_mutex_lock(&philo->forks[philo->right_fork]);
		else
			pthread_mutex_lock(&philo->forks[philo->left_fork]);
	}
}

void	*ft_philo(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if ((philo->id % 2) == 0)
	{
		excluded_printf(philo, THINK);
		usleep(60000);
	}
	while (death_check(philo) == false)
	{
		choosing_fork(philo, FIRST);
		if (fork_release(philo, 1) == true)
			return (0);
		excluded_printf(philo, FORK);
		choosing_fork(philo, SECOND);
		if (fork_release(philo, 2) == true)
			return (0);
		ft_eating(philo);
		excluded_printf(philo, SLEEP);
		smart_sleep(philo->data->time_to_sleep, philo);
		excluded_printf(philo, THINK);
	}
	return (0);
}

void	*eater(void *data)
{
	int		i;
	t_philo	*philos;

	philos = (t_philo *)data;
	usleep(60000);
	while (1)
	{
		i = 0;
		philos[i].data->exit = 0;
		while (i < philos->data->philosophers)
		{
			pthread_mutex_lock(&philos[i].data->mutexes[EATS]);
			if (philos[i].eat_times == 0)
				philos[i].data->exit++;
			pthread_mutex_unlock(&philos[i++].data->mutexes[EATS]);
		}
		if (philos->data->exit == philos->data->philosophers)
		{
			pthread_mutex_lock(&philos->data->mutexes[DEAD]);
			philos->data->someone_dead = true;
			return (pthread_mutex_unlock(&philos->data->mutexes[DEAD]), NULL);
		}
		pthread_mutex_lock(&philos->data->mutexes[DEAD]);
		if (philos->data->someone_dead == true)
			return (pthread_mutex_unlock(&philos->data->mutexes[DEAD]), NULL);
		pthread_mutex_unlock(&philos->data->mutexes[DEAD]);
	}
}

void	*killer(void *data)
{
	int		i;
	t_philo	*philos;

	philos = (t_philo *)data;
	i = 0;
	usleep(60000);
	while (1)
	{
		i = 0;
		while (i < philos->data->philosophers)
		{
			pthread_mutex_lock(&philos->data->mutexes[DEAD]);
			if (philos->data->someone_dead == true)
				return (pthread_mutex_unlock(&philos->data->mutexes[DEAD]), NULL);
			pthread_mutex_unlock(&philos->data->mutexes[DEAD]);
			pthread_mutex_lock(&philos[i].data->mutexes[TIME]);
			if (someone_starved(philos[i], philos[i].data->time_to_die))
			{
				// printf("time == [%lu]\n", (get_time() - philos[i].last_eat));
				pthread_mutex_unlock(&philos[i].data->mutexes[TIME]);
				pthread_mutex_lock(&philos->data->mutexes[DEAD]);
				philos->data->someone_dead = true;
				pthread_mutex_unlock(&philos->data->mutexes[DEAD]);
				return (final_print(&philos[i]), NULL);
			}
			pthread_mutex_unlock(&philos[i++].data->mutexes[TIME]);
		}
	}
}
