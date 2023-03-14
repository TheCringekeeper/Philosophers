/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankhabar <ankhabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 15:46:31 by ankhabar          #+#    #+#             */
/*   Updated: 2023/03/14 14:04:52 by ankhabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo1(void *data)
{
	t_data	*sdata;

	sdata = data;
	sleep(1);
	printf("\033[0;33mPhilo n_%i is thinking\033[0m\n", *sdata->n1);
	pthread_mutex_lock(sdata->vilka_levaya);
	pthread_mutex_lock(sdata->vilka_pravaya);
	printf("\033[0;31mPhilo n_%i is eating\033[0m\n", *sdata->n1);
	sleep(1);
	pthread_mutex_unlock(sdata->vilka_levaya);
	pthread_mutex_unlock(sdata->vilka_pravaya);
	printf("\033[0;30mPhilo n_%i is sleeping\033[0m\n", *sdata->n1);
	sleep(1);
	return (0);
}

void	*philo2(void *data)
{
	t_data	*sdata;

	sdata = data;
	printf("\033[0;33mPhilo n_%i is thinking\033[0m\n", *sdata->n2);
	pthread_mutex_lock(sdata->vilka_levaya);
	pthread_mutex_lock(sdata->vilka_pravaya);
	printf("\033[0;31mPhilo n_%i is eating\033[0m\n", *sdata->n2);
	sleep(1);
	pthread_mutex_unlock(sdata->vilka_levaya);
	pthread_mutex_unlock(sdata->vilka_pravaya);
	printf("\033[0;30mPhilo n_%i is sleeping\033[0m\n", *sdata->n2);
	sleep(1);
	return (0);
}

int	main(int ac, char *av[])
{
	(void)av;
	(void)ac;
	pthread_t	id1;
	pthread_t	id2;
	t_data		data;
	t_mutex		v_levaya;
	t_mutex		v_pravaya;
	int			a;
	int			b;

	a = 1;
	b = 2;
	data.n1 = &a;
	data.n2 = &b;
	data.vilka_levaya = &v_levaya;
	data.vilka_pravaya = &v_pravaya;
	// if (ac == 5 || ac == 6)
	// sozdaniye structury
	// list double, review cat eater
	// initialization time_to_ & transformation for usleep
	pthread_mutex_init(&v_levaya, 0);
	pthread_mutex_init(&v_pravaya, 0);
	// dobavit otdelniy tred dlya checker'a kto sdoh
	// s pomosh'u gettimeofday
	pthread_create(&id1, 0, philo1, &data);
	pthread_create(&id2, 0, philo2, &data);
	pthread_join(id1, 0);
	pthread_join(id2, 0);
	pthread_mutex_destroy(data.vilka_levaya);
	pthread_mutex_destroy(data.vilka_pravaya);
	return (0);
}

// void	philo_routine(int num)
// {
	
// }

// void	ft_philo(int num)
// {
// 	t_mutex		mutex;
// 	pthread_t	id;

// 	pthread_mutex_init(&mutex, 0);
// 	pthread_create(&id, 0, philo_routine, &data);
// 	pthread_join(&id, 0);
// 	pthread_mutex_destroy(&mutex);
// }

// int	main(int ac, char *av[])
// {
// 	int		index;

// 	index = 0;
// 	if (ac > 4 && ac < 7)
// 	{
// 		printf("\033[0;32mOK\n\033[0m");
// 		while (index < ft_atoi(av[1]))
// 			ft_philo(index++);
// 		return (0);
// 	}
// 	printf("\033[0;31mWrong number of arguments\n\033[0m");
// 	return (0);
// }
