/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankhabar <ankhabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 10:49:38 by ankhabar          #+#    #+#             */
/*   Updated: 2023/03/28 12:41:37 by ankhabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* fork_release() function releases the forks held by a philosopher,
** and it returns true if the philosopher is dead and cannot release
** the forks, false otherwise.*/
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

/* The choosing_fork() function is used to acquire the forks
** based on their order, either first or second, to avoid deadlocks.*/
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

/* The ft_eating() function is called by each philosopher to perform
** their eating action. It locks one fork, waits for the other fork
** to be available, and then prints a message that they have picked
** up both forks and they are eating. After that, it updates the eat
** times of the philosopher and unlocks the forks.*/
bool	ft_eating(t_philo *philo)
{
	choosing_fork(philo, FIRST);
	if (fork_release(philo, 1) == true)
		return (EXIT_FAILURE);
	excluded_printf(philo, FORK);
	if (philo->data->philosophers == 1)
	{
		pthread_mutex_unlock(&philo->forks[philo->right_fork]);
		return (EXIT_FAILURE);
	}
	choosing_fork(philo, SECOND);
	if (fork_release(philo, 2) == true)
		return (EXIT_FAILURE);
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
	return (EXIT_SUCCESS);
}

/* ft_philo() function is the main function that runs for each philosopher.
** If the philosopher's ID is even, it waits for 60 milliseconds before starting
** the while loop. Then, it calls ft_eating(), followed by sleeping and thinking
** until the philosopher is dead.*/
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
		if (ft_eating(philo) == EXIT_FAILURE)
			return (0);
		excluded_printf(philo, SLEEP);
		smart_sleep(philo->data->time_to_sleep, philo);
		if (philo->data->philosophers % 2 == 1)
			usleep(1000);
		excluded_printf(philo, THINK);
	}
	return (0);
}
