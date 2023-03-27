/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankhabar <ankhabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 15:46:31 by ankhabar          #+#    #+#             */
/*   Updated: 2023/03/27 08:03:28 by ankhabar         ###   ########.fr       */
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

bool	ft_eating(t_philo *philo)
{
	excluded_printf(philo, FORK);
	excluded_printf(philo, EAT);
	pthread_mutex_lock(&philo->data->mutexes[EATS]);
	if (philo->eat_times != -1)
		philo->eat_times--;
	pthread_mutex_unlock(&philo->data->mutexes[EATS]);
	smart_sleep(philo->data->time_to_eat, philo);
	return (true);
}

void	choosing_fork(t_philo *philo, int order)
{
	if (order == FIRST)
	{
		if (philo->left_fork < philo->right_fork)
			pthread_mutex_lock(&philo->forks[philo->left_fork]);
		else
			pthread_mutex_lock(&philo->forks[philo->right_fork]);
	}
	else if (order == SECOND)
	{
		if (philo->left_fork < philo->right_fork)
			pthread_mutex_lock(&philo->forks[philo->right_fork]);
		else
			pthread_mutex_lock(&philo->forks[philo->left_fork]);
	}
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
		choosing_fork(philo, FIRST);
		if (fork_release(philo, 1) == true)
			return (0);
		excluded_printf(philo, FORK);
		choosing_fork(philo, SECOND);
		if (fork_release(philo, 2) == true)
			return (0);
		ft_eating(philo);
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

void	*eater(void *data)
{
	int		i;
	t_philo	*philos;

	philos = (t_philo *)data;
	i = 0;
	usleep(6000);
	while (1)
	{
		i = 0;
		philos[i].data->exit = 0;
		while (i < philos->data->philosophers)
		{
			pthread_mutex_lock(&philos[i].data->mutexes[EATS]);
			if (philos[i].eat_times == 0)
				philos[i].data->exit++;
			pthread_mutex_unlock(&philos[i].data->mutexes[EATS]);
			i++;
		}
		if (philos->data->exit == philos->data->philosophers)
		{
			pthread_mutex_lock(&philos->data->mutexes[DEAD]);
			philos->data->someone_dead = true;
			pthread_mutex_unlock(&philos->data->mutexes[DEAD]);
			return (0);
		}
		pthread_mutex_lock(&philos->data->mutexes[DEAD]);
		if (philos->data->someone_dead == true)
		{
			pthread_mutex_unlock(&philos->data->mutexes[DEAD]);
			return (0);
		}
		pthread_mutex_unlock(&philos->data->mutexes[DEAD]);
	}
}

void	*killer(void *data)
{
	int		i;
	t_philo	*philos;

	philos = (t_philo *)data;
	i = 0;
	usleep(60000);
	while (1)
	{
		i = 0;
		while (i < philos->data->philosophers)
		{
			pthread_mutex_lock(&philos->data->mutexes[DEAD]);
			if (philos->data->someone_dead == true)
			{
				pthread_mutex_unlock(&philos->data->mutexes[DEAD]);
				return (0);
			}
			pthread_mutex_unlock(&philos->data->mutexes[DEAD]);
			pthread_mutex_lock(&philos[i].data->mutexes[TIME]);
			if ((get_time() - philos[i].last_eat) >= (unsigned long int)philos[i].data->time_to_die)
			{
				// printf("cur_time - last_eat time == [%lu]\n", (get_time() - philos[i].last_eat));
				pthread_mutex_unlock(&philos[i].data->mutexes[TIME]);
				pthread_mutex_lock(&philos->data->mutexes[DEAD]);
				philos->data->someone_dead = true;
				pthread_mutex_unlock(&philos->data->mutexes[DEAD]);
				final_print(&philos[i]);
				return (0);
			}
			pthread_mutex_unlock(&philos[i].data->mutexes[TIME]);
			i++;
		}
	}
}

void	ft_mutex_destroy(t_philo *philo)
{
	int	i;

	i = -1;
	while (++i < philo->data->philosophers)
		pthread_mutex_destroy(&philo->forks[i]);
	i = -1;
	while (++i < M_NUM)
		pthread_mutex_destroy(&philo->data->mutexes[i]);
}

void	init_pthreads(t_philo *philos)
{
	int			index;
	pthread_t	*id;
	pthread_t	kill;
	pthread_t	eat;

	kill = -1;
	eat = -2;
	id = malloc(sizeof(pthread_t) * philos->data->philosophers);
	if (id == NULL)
		return ;
	index = 0;
	while (index < philos->data->philosophers)
	{
		pthread_create(&id[index], 0, ft_philo, (void *)&philos[index]);
		index++;
	}
	pthread_create(&kill, 0, killer, (void *)&philos[0]);
	if (philos->data->must_eat != -1)
		pthread_create(&eat, 0, eater, (void *)&philos[0]);
	usleep(1000);
	if (philos->data->must_eat != -1)
		pthread_join(eat, 0);
	pthread_join(kill, 0);
	index = 0;
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
		free_everything(philos);
	}
	return (0);
}
