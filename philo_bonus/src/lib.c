/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:07:50 by abaudot           #+#    #+#             */
/*   Updated: 2021/08/23 17:41:32 by abaudot          ###   ########.fr       */
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

uint32_t	ft_buffnbr(uint32_t n, char *buf, uint32_t end)
{
	uint32_t	i;

	i = end;
	if (n == 0)
		buf[--i] = '0';
	while (n)
	{
		buf[--i] = '0' + (n % 10);
		n /= 10;
	}
	return (end - i);
}

long	get_timestamp(void)
{
	static struct timeval	tv;

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
