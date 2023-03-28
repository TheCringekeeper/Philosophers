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

/* free_everything function is responsible for freeing all
** dynamically allocated memory in the program */
void	free_everything(t_philo *philos)
{
	if (philos != NULL && philos->data != NULL && philos->data->mutexes != NULL)
		free(philos->data->mutexes);
	if (philos != NULL && philos->data != NULL)
		free(philos->data);
	if (philos != NULL && philos->forks != NULL)
		free(philos->forks);
	if (philos != NULL)
		free(philos);
}
