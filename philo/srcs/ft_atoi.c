/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 16:12:52 by ulysse            #+#    #+#             */
/*   Updated: 2023/03/29 19:48:50 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* This function transforms ascii string to a long long int.*/
long long	ft_atoi(const char *nbr)
{
	long long	res;
	int	sign;

	res = 0;
	sign = 1;
	while ((*nbr == ' ') || ('\t' <= *nbr && *nbr <= '\r'))
		nbr++;
	if (*nbr == '+' || *nbr == '-')
	{
		if (*nbr == '-')
			sign *= -1;
		nbr++;
	}
	while ('0' <= *nbr && *nbr <= '9')
	{
		res = res * 10 + *nbr - '0';
		nbr++;
	}
	res *= sign;
	return (res);
}
