/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 15:49:24 by ankhabar          #+#    #+#             */
/*   Updated: 2023/03/25 14:18:40 by marvin           ###   ########.fr       */
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
	EATS,
	TIME,
	DEAD,
	SYNK,
	M_NUM
}	t_mutexes;

# define FORK "has taken a fork"
# define THINK "is thinking"
# define EAT "is eating"
# define SLEEP "is sleeping"
# define DIED "died"
# define MESSAGE1 "=======test 1111111========"
# define MESSAGE2 "=======test 2222222========"

typedef pthread_mutex_t	t_mutex;

/* data that never changes */
typedef struct s_data {
	u_int64_t	sim_start;
	int			philosophers;
	u_int64_t	time_to_die;
	u_int64_t	time_to_eat;
	u_int64_t	time_to_sleep;
	bool		someone_dead;
	int			must_eat;
	t_mutex		*mutexes;
}				t_data;

/* t_data is just to access data any time */
typedef struct s_philo {
	u_int64_t	last_eat;
	int			id;
	int			left_fork;
	int			right_fork;
	t_mutex		*forks;
	t_data		*data;
}				t_philo;

///////////////////////init/////////////////////

t_mutex		*init_mutexes(void);
t_philo		*init_struct(int ac, char *av[]);

///////////////////////time/////////////////////

u_int64_t	get_time(void);
void		excluded_printf(t_philo *philo, char *code);
void		ft_usleep(int to_sleep);
void		final_print(t_philo *philo);
void		smart_sleep(u_int64_t time, t_philo *philo);
bool		someone_died(t_philo *philo);

//////////////////////ft_atoi///////////////////

long long	ft_atoi(const char *nptr);
void		free_everything(t_philo *philos);

#endif
