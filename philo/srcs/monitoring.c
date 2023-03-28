/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankhabar <ankhabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 09:26:22 by ankhabar          #+#    #+#             */
/*   Updated: 2023/03/28 12:33:44 by ankhabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* The meal_monitoring function is responsible for
** monitoring the number of times each philosopher
** has eaten and checking whether all philosophers
** have eaten the required number of times. If all
** philosophers have eaten the required number of
** times, the function calls the kill_philo function
** and exits the thread.*/
void	*meal_monitoring(void *data)
{
	int		i;
	int		nb_fed_up_philo;
	t_philo	*philos;

	philos = (t_philo *)data;
	usleep(60000);
	while (death_check(philos) == false)
	{
		i = 0;
		nb_fed_up_philo = 0;
		while (i < philos->data->philosophers)
		{
			pthread_mutex_lock(&philos->data->mutexes[READ]);
			if (philos[i].eat_times >= philos->data->must_eat)
				nb_fed_up_philo++;
			pthread_mutex_unlock(&philos->data->mutexes[READ]);
			i++;
		}
		usleep(100);
		if (nb_fed_up_philo == philos->data->philosophers)
			return (kill_philo(philos), NULL);
	}
	return (NULL);
}

/* The death_monitoring function is responsible
** for monitoring each philosopher's time to live
** and checking whether any philosopher has starved.
** If a philosopher has starved, the function calls
** the kill_philo function and then calls the final_print
** function to print the final status of the philosopher.
** The function continues to execute until the death_check
** function returns true.*/
void	*death_monitoring(void *data)
{
	int		i;
	t_philo	*philos;

	philos = (t_philo *)data;
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
				kill_philo(philos);
				return (final_print(&philos[i]), NULL);
			}
			pthread_mutex_unlock(&philos->data->mutexes[READ]);
			i++;
		}
		usleep(1000);
	}
}
