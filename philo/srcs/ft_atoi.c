/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankhabar <ankhabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 16:12:52 by ulysse            #+#    #+#             */
/*   Updated: 2023/03/14 14:22:06 by ankhabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	ft_atoi(const char *nptr)
{
	long long	res;
	long long	signe;

	signe = 1;
	while ((*nptr == 32) || (9 <= *nptr && *nptr <= 13))
		nptr++;
	if (*nptr == 43 || *nptr == 45)
	{
		if (*nptr == 45)
			signe = -1;
		nptr++;
	}
	res = 0;
	while (47 < *nptr && *nptr < 58)
	{
		res = res * 10 + *nptr - 48;
		nptr++;
	}
	res *= signe;
	return (res);
}
