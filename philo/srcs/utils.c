/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankhabar <ankhabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 09:20:41 by ankhabar          #+#    #+#             */
/*   Updated: 2023/03/28 05:14:50 by ankhabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	final_print(t_philo *philo)
{
	long unsigned int	stamp;

	stamp = timestamp(philo->data->sim_start);
	printf("%lu %i %s\n", stamp, philo->id, DIED);
}

void	excluded_printf(t_philo *philo, char *code)
{
	long unsigned int	stamp;

	stamp = timestamp(philo->data->sim_start);
	pthread_mutex_lock(&philo->data->mutexes[DEAD]);
	if (philo->data->should_exit == false)
		printf("%lu %i %s\n", stamp, philo->id, code);
	pthread_mutex_unlock(&philo->data->mutexes[DEAD]);
}

bool	someone_starved(t_philo philo, u_int64_t time_to_die)
{
	return ((get_time() - philo.last_eat) >= time_to_die);
}

bool	death_check(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutexes[DEAD]);
	if (philo->data->should_exit == true)
		return (pthread_mutex_unlock(&philo->data->mutexes[DEAD]), true);
	return (pthread_mutex_unlock(&philo->data->mutexes[DEAD]), false);
}
