/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankhabar <ankhabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 15:46:31 by ankhabar          #+#    #+#             */
/*   Updated: 2023/03/28 09:17:30 by ankhabar         ###   ########.fr       */
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
	pthread_create(&kill, 0, killer, (void *)philos);
	if (philos->data->must_eat != -1)
		pthread_create(&eat, 0, eater, (void *)philos);
	usleep(1000);
	index = -1;
	while (++index < philos->data->philosophers)
		pthread_join(id[index], 0);
	pthread_join(kill, 0);
	if (philos->data->must_eat != -1)
		pthread_join(eat, 0);
	// ft_mutex_destroy(philos);
	free(id);
}

// to add: destroy mutexes
int	main(int ac, char *av[])
{
	t_philo	*philos;

	if (ac > 4 && ac < 7)
	{
		philos = init_struct(ac, av);
		if (philos == NULL)
		{
			write(2, "Error\nInvalid input\n", 20);
			return (1);
		}
		if (philos->data->must_eat != 0)
			init_pthreads(philos);
		else
			write(2, "Warning\n'notepme' is set to 0\n", 30);
		free_everything(philos);
	}
	return (0);
}
