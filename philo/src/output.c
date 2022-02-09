/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:12:15 by abaudot           #+#    #+#             */
/*   Updated: 2022/02/09 17:50:12 by abaudot          ###   ########.fr       */
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

void	annonce(const t_philo *philo, const char *message)
{
	static char			buff[65536];
	static char			time[64] = "\033[1;36m-----------\033[0;0m\t";
	static uint32_t		bsize;

	if (philo->perspective->someone_die)
		return ;
	ft_buffnbr(get_timestamp() - philo->perspective->time_start, time, 18);
	ft_strncpy(buff + bsize, time, 26);
	bsize += 26;
	ft_strncpy(buff + bsize, philo->str_name, philo->n_name);
	bsize += philo->n_name;
	buff[bsize++] = '\t';
	ft_strncpy(buff + bsize, message, LEN);
	bsize += LEN;
	if ((message[11] != 'e') & (message[8] != 'd'))
		return ;
	(void)!write(1, buff, bsize);
	bsize = 0;
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
