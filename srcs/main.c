/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 15:46:31 by ankhabar          #+#    #+#             */
/*   Updated: 2023/03/25 16:42:58 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	fork_release(struct s_philo *philo, int forks)
{
	if (someone_died(philo) == true)
	{
		pthread_mutex_unlock(&philo->forks[philo->left_fork]);
		if (forks == 2)
			pthread_mutex_unlock(&philo->forks[philo->right_fork]);
		return (true);
	}
	return (false);
}

void	*ft_philo(void *data)
{
	bool	death;
	t_philo	*philo;

	philo = (t_philo *)data;
	if ((philo->id % 2) == 0)
	{
		excluded_printf(philo, THINK);
		usleep(60000);
	}
	death = someone_died(philo);
	while (death == false)
	{
		if (someone_died(philo) == true)
			break ;
		pthread_mutex_lock(&philo->forks[philo->left_fork]);
		if (fork_release(philo, 1) == true)
			return (0);
		excluded_printf(philo, FORK);
		pthread_mutex_lock(&philo->forks[philo->right_fork]);
		if (fork_release(philo, 2) == true)
			return (0);
		excluded_printf(philo, FORK);
		excluded_printf(philo, EAT);
		smart_sleep(philo->data->time_to_eat, philo);
		pthread_mutex_unlock(&philo->forks[philo->left_fork]);
		pthread_mutex_unlock(&philo->forks[philo->right_fork]);
		pthread_mutex_lock(&philo->data->mutexes[TIME]);
		philo->last_eat = get_time();
		pthread_mutex_unlock(&philo->data->mutexes[TIME]);
		excluded_printf(philo, SLEEP);
		smart_sleep(philo->data->time_to_sleep, philo);
		excluded_printf(philo, THINK);
		death = someone_died(philo);
	}
	return (0);
}

void	set_counters(t_philo *philos, t_data *data)
{
	int	i;

	pthread_mutex_lock(&data->mutexes[SYNK]);
	data->sim_start = get_time();
	i = 0;
	while (i < philos->data->philosophers)
	{
		pthread_mutex_lock(&data->mutexes[EATS]);
		philos[i].last_eat = data->sim_start;
		pthread_mutex_unlock(&data->mutexes[EATS]);
		i++;
	}
	pthread_mutex_unlock(&data->mutexes[SYNK]);
}

void	killer(t_philo *philos)
{
	int		i;

	i = 0;
	usleep(60000);
	while (1)
	{
		i = 0;
		while (i < philos->data->philosophers)
		{
			pthread_mutex_lock(&philos[i].data->mutexes[TIME]);
			if ((get_time() - philos[i].last_eat) >= (unsigned long int)philos[i].data->time_to_die)
			{
				pthread_mutex_unlock(&philos[i].data->mutexes[TIME]);
				pthread_mutex_lock(&philos->data->mutexes[DEAD]);
				philos->data->someone_dead = true;
				pthread_mutex_unlock(&philos->data->mutexes[DEAD]);
				final_print(&philos[i]);
				return ;
			}
			pthread_mutex_unlock(&philos[i].data->mutexes[TIME]);
			i++;
		}
	}
}

// void	ft_mutex_destroy(t_philo *philo)
// {
// 	int	i;

// 	i = -1;
// 	while (++i < philo->data->philosophers)
// 		pthread_mutex_destroy(&philo->forks[i]);
// 	i = -1;
// 	while (++i < M_NUM)
// 		pthread_mutex_destroy(&philo->data->mutexes[i]);
// }

void	init_pthreads(t_philo *philos)
{
	int			index;
	pthread_t	*id;

	index = 0;
	id = malloc(sizeof(pthread_t) * philos->data->philosophers);
	if (id == NULL)
		return ;
	while (index < philos->data->philosophers)
	{
		pthread_create(&id[index], 0, ft_philo, (void *)&philos[index]);
		index++;
	}
	// set_counters(philos, philos->data);
	killer(philos);
	usleep(1000);
	while (index < philos->data->philosophers)
		pthread_join(id[index++], 0);
	// ft_mutex_destroy(philos);
	free(id);
}

int	main(int ac, char *av[])
{
	t_philo	*philos;

	if (ac > 4 && ac < 7)
	{
		philos = init_struct(ac, av);
		init_pthreads(philos);
	}
	free_everything(philos);
	return (0);
}
