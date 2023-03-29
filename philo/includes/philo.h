/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 15:49:24 by ankhabar          #+#    #+#             */
/*   Updated: 2023/03/29 20:13:37 by marvin           ###   ########.fr       */
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
	READ,
	EXIT,
	M_NUM
}	t_mutexes;

# define FIRST 1
# define SECOND 2
# define FORK "has taken a fork"
# define THINK "is thinking"
# define EAT "is eating"
# define SLEEP "is sleeping"
# define DIED "died"

typedef pthread_mutex_t	t_mutex;

/* Data shared by all philosophers.*/
typedef struct s_data {
	u_int64_t	sim_start;
	int			philosophers;
	long long	time_to_die;
	long long	time_to_eat;
	long long	time_to_sleep;
	bool		should_exit;
	int			must_eat;
	t_mutex		*mutexes;
}				t_data;

/* Personal data of each philosopher.*/
typedef struct s_philo {
	u_int64_t	last_meal;
	int			id;
	int			eat_times;
	int			left_fork;
	int			right_fork;
	t_mutex		*forks;
	t_data		*data;
}				t_philo;

///////////////////////free/////////////////////
void		free_everything(t_philo *philos);

//////////////////////ft_atoi///////////////////
long long	ft_atoi(const char *nbr);

/////////////////////ft_philo///////////////////
void		*ft_philo(void *data);

///////////////////////init/////////////////////
t_philo		*init_struct(int ac, char *av[]);

////////////////////monitoring//////////////////
void		*meal_monitoring(void *data);
void		*death_monitoring(void *data);

///////////////////////time/////////////////////
u_int64_t	get_time(void);
u_int64_t	timestamp(u_int64_t sim_start);
void		smart_sleep(u_int64_t time, t_philo *philo);

///////////////////////utils////////////////////
void		print_death(t_philo *philo);
void		excluded_printf(t_philo *philo, char *code);
bool		someone_starved(t_philo philo, u_int64_t time_to_die);
bool		exit_check(t_philo *philo);
void		kill_philo(t_philo *philo);

#endif
