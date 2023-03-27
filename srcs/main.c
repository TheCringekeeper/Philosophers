/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankhabar <ankhabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 15:46:31 by ankhabar          #+#    #+#             */
/*   Updated: 2023/03/27 09:51:18 by ankhabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	pthread_t	kill;
	pthread_t	eat;

	kill = -1;
	eat = -2;
	id = malloc(sizeof(pthread_t) * philos->data->philosophers);
	if (id == NULL)
		return ;
	index = -1;
	while (++index < philos->data->philosophers)
		pthread_create(&id[index], 0, ft_philo, (void *)&philos[index]);
	pthread_create(&kill, 0, killer, (void *)&philos[0]);
	if (philos->data->must_eat != -1)
		pthread_create(&eat, 0, eater, (void *)&philos[0]);
	// usleep(1000);
	if (philos->data->must_eat != -1)
		pthread_join(eat, 0);
	pthread_join(kill, 0);
	index = -1;
	while (++index < philos->data->philosophers)
		pthread_join(id[index], 0);
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
