/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankhabar <ankhabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 09:26:22 by ankhabar          #+#    #+#             */
/*   Updated: 2023/03/28 08:37:35 by ankhabar         ###   ########.fr       */
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

void	ft_eating(t_philo *philo)
{
	excluded_printf(philo, FORK);
	excluded_printf(philo, EAT);
	pthread_mutex_lock(&philo->data->mutexes[READ]);
	if (philo->eat_times != -1)
		philo->eat_times++;
	pthread_mutex_unlock(&philo->data->mutexes[READ]);
	philo->last_eat = get_time();
	smart_sleep(philo->data->time_to_eat, philo);
	pthread_mutex_unlock(&philo->forks[philo->left_fork]);
	pthread_mutex_unlock(&philo->forks[philo->right_fork]);
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
		else if (philo->data->philosophers == 1)
			return ;
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
		if (philo->data->philosophers == 1)
			return (pthread_mutex_unlock(&philo->forks[philo->right_fork]), NULL);
		if (fork_release(philo, 2) == true)
			return (0);
		ft_eating(philo);
		excluded_printf(philo, SLEEP);
		smart_sleep(philo->data->time_to_sleep, philo);
		if (philo->data->philosophers % 2 == 1)
			usleep(1000);
		excluded_printf(philo, THINK);
	}
	return (0);
}

void	*eater(void *data)
{
	int		i;
	int		nb_fed_up_philo;
	t_philo	*philos;

	philos = (t_philo *)data;
	usleep(60000);
	while (1)
	{
		i = 0;
		nb_fed_up_philo = 0;
		while (i < philos->data->philosophers)
		{
			pthread_mutex_lock(&philos->data->mutexes[READ]);
			if (philos[i].eat_times == philos->data->must_eat)
				nb_fed_up_philo++;
			pthread_mutex_unlock(&philos->data->mutexes[READ]);
			i++;
		}
		usleep(100);
		if (nb_fed_up_philo == philos->data->philosophers)
		{
			pthread_mutex_lock(&philos->data->mutexes[DEAD]);
			philos->data->should_exit = true;
			return (pthread_mutex_unlock(&philos->data->mutexes[DEAD]), NULL);
		}
		if (death_check(philos) == true)
			return (NULL);
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
			if (death_check(philos) == true)
				return (NULL);
			pthread_mutex_lock(&philos->data->mutexes[READ]);
			if (someone_starved(philos[i], philos[i].data->time_to_die))
			{
				pthread_mutex_unlock(&philos->data->mutexes[READ]);
				pthread_mutex_lock(&philos->data->mutexes[DEAD]);
				philos->data->should_exit = true;
				pthread_mutex_unlock(&philos->data->mutexes[DEAD]);
				return (final_print(&philos[i]), NULL);
			}
			pthread_mutex_unlock(&philos->data->mutexes[READ]);
			i++;
		}
		usleep(1000);
	}
}
