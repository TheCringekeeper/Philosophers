/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 21:00:41 by ankhabar          #+#    #+#             */
/*   Updated: 2023/03/21 22:59:27 by marvin           ###   ########.fr       */
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

static u_int64_t	timestamp(t_philo *philo, u_int64_t sim_start)
{
	u_int64_t	test;

	pthread_mutex_lock(&philo->data->mutexes[TIME]);
	test = get_time() - sim_start;
	pthread_mutex_unlock(&philo->data->mutexes[TIME]);
	return (test);
}

void	final_print(t_philo *philo)
{
	long unsigned int	stamp;

	stamp = timestamp(philo, philo->data->sim_start);
	pthread_mutex_lock(&philo->data->mutexes[PRINT]);
	printf("%lu %i %s\n", stamp, philo->id, DIED);
	pthread_mutex_unlock(&philo->data->mutexes[PRINT]);
}

void	excluded_printf(t_philo *philo, char *code)
{
	long unsigned int	stamp;

	stamp = timestamp(philo, philo->data->sim_start);
	pthread_mutex_lock(&philo->data->mutexes[DEAD]);
	if (philo->data->someone_dead ==false)
	{
		pthread_mutex_lock(&philo->data->mutexes[PRINT]);
		printf("%lu %i %s\n", stamp, philo->id, code);
		pthread_mutex_unlock(&philo->data->mutexes[PRINT]);
	}
	pthread_mutex_unlock(&philo->data->mutexes[DEAD]);
}

void	ft_usleep(int to_sleep)
{
	usleep(to_sleep * 1000);
}

void	smart_sleep(int time, t_philo *philo)
{
	u_int64_t start;

	start = get_time();
	while (philo->dead == false)
	{
		pthread_mutex_lock(&philo->data->mutexes[DEAD]);
		if (philo->dead == true)
		{
			pthread_mutex_unlock(&philo->data->mutexes[DEAD]);
			break ;
		}
		pthread_mutex_unlock(&philo->data->mutexes[DEAD]);
		if ((get_time() - start) >= (u_int64_t)time)
			break ;
		usleep(50);
	}
}

