/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:07:50 by abaudot           #+#    #+#             */
/*   Updated: 2022/02/09 21:25:21 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

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

long	get_timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (((tv.tv_sec * 1000) + tv.tv_usec * 0.001));
}

void	ft_usleep(int duration)
{
	const long	start = get_timestamp();
	long		current;

	current = 0;
	while (duration > current)
	{
		usleep(100);
		current = get_timestamp() - start;
	}
}
