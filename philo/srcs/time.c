/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 21:00:41 by ankhabar          #+#    #+#             */
/*   Updated: 2023/03/29 18:58:59 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* get_time is used to calculate current time using gettimeofday function */
u_int64_t	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (0);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/* timestamp is used to print the time of an event in the simulation */
u_int64_t	timestamp(u_int64_t sim_start)
{
	return (get_time() - sim_start);
}

/* smart_sleep is used to pause the execution of a thread
** for a specified time while ensuring that the program
** should not exit due to the death of a philosopher */
void	smart_sleep(u_int64_t sleep_time, t_philo *philo)
{
	u_int64_t	start;

	start = get_time();
	while (get_time() - start < sleep_time)
	{
		if (exit_check(philo) == true)
			return ;
		usleep(10);
	}
}
