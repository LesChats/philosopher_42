/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:07:50 by abaudot           #+#    #+#             */
/*   Updated: 2021/08/16 12:04:17 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

uint32_t	ft_atoi(const char *s)
{
	uint32_t	tmp;
	uint32_t	res;

	res = 0;
	while (1)
	{
		tmp = (uint32_t)((*s) - '0');
		if (tmp > 9)
			return (res);
		res = res * 10 + tmp;
		++s;
	}
}

uint32_t	get_time(struct timeval *tv)
{
	struct timeval			current_time;

	gettimeofday(&current_time, NULL);
	return ((int)(current_time.tv_sec - tv->tv_sec) *1000
			+ (int)(current_time.tv_usec - tv->tv_usec) *0.001);
}
