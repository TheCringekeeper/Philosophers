/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 09:26:22 by ankhabar          #+#    #+#             */
/*   Updated: 2023/03/29 19:11:35 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* I added two small usleep() functions after each while loop in these two
** threads to let the program work well with valgrind --tool=helgrind, otherwise
** it stucks inside these threads interrupting ft_philo threads to run.*/

/* The meal_monitoring function is responsible for monitoring the number of
** times each philosopher has eaten and checking whether all philosophers have
** eaten the required number of times. If all philosophers have eaten the
** required number of times, the function calls the kill_philo function and
** exits the thread.*/
void	*meal_monitoring(void *data)
{
	int		i;
	int		fed_up_philos;
	t_philo	*philos;

	philos = (t_philo *)data;
	usleep(60000);
	while (exit_check(philos) == false)
	{
		i = 0;
		fed_up_philos = 0;
		while (i < philos->data->philosophers)
		{
			pthread_mutex_lock(&philos->data->mutexes[READ]);
			if (philos[i].eat_times >= philos->data->must_eat)
				fed_up_philos++;
			pthread_mutex_unlock(&philos->data->mutexes[READ]);
			i++;
		}
		usleep(100);
		if (fed_up_philos == philos->data->philosophers)
			return (kill_philo(philos), NULL);
	}
	return (EXIT_SUCCESS);
}

/* The death_monitoring function is responsible for monitoring each
** philosopher's time to live and checking whether any philosopher
** has starved. If a philosopher has starved, the function calls the
** kill_philo function and then calls the print_death function to print
** the death of the philosopher. The function continues to execute
** until the exit_check function returns true.*/
void	*death_monitoring(void *data)
{
	int		i;
	t_philo	*philos;

	philos = (t_philo *)data;
	usleep(60000);
	while (exit_check(philos) == false)
	{
		i = 0;
		while (i < philos->data->philosophers)
		{
			pthread_mutex_lock(&philos->data->mutexes[READ]);
			if (someone_starved(philos[i], philos[i].data->time_to_die))
			{
				pthread_mutex_unlock(&philos->data->mutexes[READ]);
				kill_philo(philos);
				return (print_death(&philos[i]), NULL);
			}
			pthread_mutex_unlock(&philos->data->mutexes[READ]);
			i++;
		}
		usleep(100);
	}
	return (EXIT_SUCCESS);
}
