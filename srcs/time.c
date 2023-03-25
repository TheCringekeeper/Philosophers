/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 21:00:41 by ankhabar          #+#    #+#             */
/*   Updated: 2023/03/25 12:10:33 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

u_int64_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

static u_int64_t	timestamp(u_int64_t sim_start)
{
	return (get_time() - sim_start);
}

void	final_print(t_philo *philo)
{
	long unsigned int	stamp;

	stamp = timestamp(philo->data->sim_start);
	pthread_mutex_lock(&philo->data->mutexes[PRINT]);
	printf("%lu %i %s\n", stamp, philo->id, DIED);
	pthread_mutex_unlock(&philo->data->mutexes[PRINT]);
}

void	excluded_printf(t_philo *philo, char *code)
{
	long unsigned int	stamp;

	stamp = timestamp(philo->data->sim_start);
	pthread_mutex_lock(&philo->data->mutexes[DEAD]);
	if (philo->data->someone_dead == false)
	{
		// pthread_mutex_lock(&philo->data->mutexes[PRINT]);
		printf("%lu %i %s\n", stamp, philo->id, code);
		// pthread_mutex_unlock(&philo->data->mutexes[PRINT]);
	}
	pthread_mutex_unlock(&philo->data->mutexes[DEAD]);
}

void	ft_usleep(int to_sleep)
{
	usleep(to_sleep * 1000);
}

int	death_check(t_philo *philo)
{
	int	ret;

	pthread_mutex_lock(&philo->data->mutexes[DEAD]);
	if (philo->dead == true)
		ret = 1;
	else
		ret = 0;
	pthread_mutex_unlock(&philo->data->mutexes[DEAD]);
	return (ret);
}

void	smart_sleep(int time, t_philo *philo)
{
	u_int64_t start;

	start = get_time();
	while (1)
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
		usleep(1);
	}
}

