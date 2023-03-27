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
	if (philos->data->mutexes != NULL)
		free(philos->data->mutexes);
	if (philos->data != NULL)
		free(philos->data);
	if (philos->forks != NULL)
		free(philos->forks);
	if (philos != NULL)
		free(philos);
}
