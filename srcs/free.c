/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 23:10:59 by marvin            #+#    #+#             */
/*   Updated: 2023/03/21 23:10:59 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_everything(t_philo *philos)
{
	usleep(1000);
	free(philos->data->mutexes);
	free(philos->data);
	free(philos->forks);
	free(philos);
}