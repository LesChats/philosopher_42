/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:12:15 by abaudot           #+#    #+#             */
/*   Updated: 2022/01/31 15:29:27 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static void	ft_strncpy(char *dst, const char *src, const uint32_t n)
{
	uint32_t	i;

	i = 0;
	while (i < n)
	{
		dst[i] = src[i];
		++i;
	}
}

static uint32_t	ft_buffnbr(uint32_t n, char *buf, uint32_t end)
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

static uint32_t	num_size(uint32_t num)
{
	return (1 + (num >= 10) + (num >= 100) + (num >= 1000) + (num >= 10000)
		+ (num >= 100000) + (num >= 1000000) + (num >= 10000000));
}

void	annonce(const t_philo *philo, const char *message)
{
	static char	buff[64] = "\033[1;36m-----------\033[0;0m\t";
	uint32_t	i;

	if (philo->perspective->someone_die || philo->has_finished)
		return ;
	i = 26;
	ft_buffnbr(get_timestamp() - philo->perspective->time_start, buff, 18);
	i += ft_buffnbr(philo->name, buff + i, num_size(philo->name));
	buff[i++] = '\t';
	ft_strncpy(buff + i, message, LEN);
	i += LEN;
	write(1, buff, i);
}

/*
** For those using tester or over concervative about the output
** Pair whit change in .h file
**
void	annonce(const t_philo *philo, const char *message)
{
	static char	buff[64] = "-----------\t";
	uint32_t	i;

	i = 13;
	ft_buffnbr(get_time(&philo->perspective->time_start), buff, 11);
	i += ft_buffnbr(philo->name, buff + i, num_size(philo->name));
	buff[i++] = '\t';
	ft_strncpy(buff + i, message, LEN);
	i += LEN;
	write(1, buff, i);
}
*/
