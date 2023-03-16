/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankhabar <ankhabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 15:49:24 by ankhabar          #+#    #+#             */
/*   Updated: 2023/03/16 14:37:48 by ankhabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <pthread.h>

typedef enum e_mutexes
{
	PRINT,
	MEALS,
	DONE,
	DEAD,
	M_NUM
}	t_mutexes;

# define FORK "has taken a fork"
# define THINK "is thinking"
# define EAT "is eating"
# define SLEEP "is sleeping"
# define DIED "died"
# define MESSAGE "someone is really dead"

typedef	pthread_mutex_t	t_mutex;

typedef	struct s_data {
	u_int64_t	start_time;
	int			philosophers;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			must_eat;
	t_mutex		*mutexes;
}				t_data;

/* t_data is just to access data at any time */

typedef struct s_philo {
	int		id;
	int		left_fork; //self id
	int		right_fork; //next philo id
	int		dead;
	t_mutex	*forks;
	t_data	*data;
}			t_philo;

//////////////////////ft_atoi///////////////////

long long	ft_atoi(const char *nptr);

#endif
