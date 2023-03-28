/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankhabar <ankhabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 21:00:41 by ankhabar          #+#    #+#             */
/*   Updated: 2023/03/28 07:17:18 by ankhabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

u_int64_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

u_int64_t	timestamp(u_int64_t sim_start)
{
	return (get_time() - sim_start);
}

void	smart_sleep(u_int64_t sleep_time, t_philo *philo)
{
	u_int64_t	start;

	start = get_time();
	while (1)
	{
		if (get_time() - start >= sleep_time)
			return ;
		if (death_check(philo) == true)
			return ;
		usleep(10);
	}
}
