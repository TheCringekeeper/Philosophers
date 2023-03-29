/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 09:20:41 by ankhabar          #+#    #+#             */
/*   Updated: 2023/03/29 18:58:59 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* This function is responsible for printing the final message of a philosopher
** who has died. It takes a philosopher as an argument and uses the timestamp
** function to get the current time stamp. Then, it prints the time stamp,
** the philosopher's ID, and the message "died".*/
void	print_death(t_philo *philo)
{
	long unsigned int	stamp;

	stamp = timestamp(philo->data->sim_start);
	printf("%lu %i %s\n", stamp, philo->id, DIED);
}

/* This function is responsible for printing messages related to
** the philosophers' activities during the simulation. It takes
** a philosopher and a message code as arguments. It uses the timestamp
** function to get the current time stamp. It also uses a mutex to ensure
** that only one thread can write to the console at a time.*/
void	excluded_printf(t_philo *philo, char *code)
{
	long unsigned int	stamp;

	pthread_mutex_lock(&philo->data->mutexes[EXIT]);
	stamp = timestamp(philo->data->sim_start);
	if (philo->data->should_exit == false)
		printf("%lu %i %s\n", stamp, philo->id, code);
	pthread_mutex_unlock(&philo->data->mutexes[EXIT]);
}

/* This function is used to check whether a philosopher has starved to death.
** It takes a philosopher and his time_to_die as arguments. It calculates
** the time elapsed since this philosopher's last_meal and returns true if
** it's greater than or equal to the time_to_die.*/
bool	someone_starved(t_philo philo, u_int64_t time_to_die)
{
	return ((get_time() - philo.last_meal) >= time_to_die);
}

/* This function is used to check whether the simulation should exit due to a
** philosopher's death. It takes a philosopher as an argument and uses a mutex
** to check whether the should_exit flag is true. If it is, the function
** returns true, indicating that the simulation should exit.*/
bool	exit_check(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutexes[EXIT]);
	if (philo->data->should_exit == true)
		return (pthread_mutex_unlock(&philo->data->mutexes[EXIT]), true);
	return (pthread_mutex_unlock(&philo->data->mutexes[EXIT]), false);
}

/* This function is used to mark a philosopher as dead. It takes a philosopher
** as an argument and uses a mutex to set the should_exit flag to true.*/
void	kill_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutexes[EXIT]);
	philo->data->should_exit = true;
	pthread_mutex_unlock(&philo->data->mutexes[EXIT]);
}
