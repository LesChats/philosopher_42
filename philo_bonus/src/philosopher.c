/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:18:19 by abaudot           #+#    #+#             */
/*   Updated: 2022/02/09 16:33:40 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

static uint8_t	free_assage(t_philo **philos, const uint32_t n)
{
	uint32_t	i;

	if (!(*philos) || !n)
		return (1);
	sem_close((*philos)->display);
	sem_close((*philos)->forks);
	sem_close((*philos)->kill_table);
	i = 0;
	while (i < n)
	{
		sem_close(((*philos) + i)->eat_sem);
		++i;
	}
	free(*philos);
	return (0);
}

static uint8_t	print_usage(void)
{
	printf ("%sError:%s Wrong usage\n", RED, EOC);
	printf ("%sUsage%s ./philo [num on philo] [time to die]", YELLOW, EOC);
	printf (" [time to eat] [time to sleep] %soptional:%s", BLUE, EOC);
	printf (" [num of meal]\n");
	return (1);
}

int	main(int ac, char **av)
{
	struct s_the_table	table;
	t_philo				*philos;

	if (ac != 5 && ac != 6)
		return (print_usage());
	if (dress_table(&table, &philos, av, ac))
		return (free_assage(&philos, table.n_philo));
	start_dinner(philos);
	free_assage(&philos, table.n_philo);
	return (0);
}
