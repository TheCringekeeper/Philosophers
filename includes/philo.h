/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankhabar <ankhabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 15:49:24 by ankhabar          #+#    #+#             */
/*   Updated: 2023/03/14 14:05:40 by ankhabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>

typedef	pthread_mutex_t	t_mutex;

typedef struct s_data {
	t_mutex	*vilka_levaya;
	t_mutex	*vilka_pravaya;
	int		*n1;
	int		*n2;
}			t_data;

#endif
