/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankhabar <ankhabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 21:00:41 by ankhabar          #+#    #+#             */
/*   Updated: 2023/03/27 04:32:22 by ankhabar         ###   ########.fr       */
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
	printf("%lu %i %s\n", stamp, philo->id, DIED);
}

void	excluded_printf(t_philo *philo, char *code)
{
	long unsigned int	stamp;

	stamp = timestamp(philo->data->sim_start);
	pthread_mutex_lock(&philo->data->mutexes[DEAD]);
	if (philo->data->someone_dead == false)
		printf("%lu %i %s\n", stamp, philo->id, code);
	pthread_mutex_unlock(&philo->data->mutexes[DEAD]);
}

void	ft_usleep(int to_sleep)
{
	usleep(to_sleep * 1000);
}

bool	someone_died(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutexes[DEAD]);
	if (philo->data->someone_dead == true)
		return (pthread_mutex_unlock(&philo->data->mutexes[DEAD]), true);
	return (pthread_mutex_unlock(&philo->data->mutexes[DEAD]), false);
}

void	smart_sleep(u_int64_t sleep_time, t_philo *philo)
{
	u_int64_t	start;

	start = get_time();
	while (1)
	{
		// printf("sleep time == [%lu], cur_time - start time == [%lu]\n", sleep_time, (get_time() - start));
		if (get_time() - start >= sleep_time)
			return ;
		if (someone_died(philo) == true)
			return ;
		usleep(10);
	}
}
