/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo_table.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:07:40 by abaudot           #+#    #+#             */
/*   Updated: 2021/08/23 17:29:37 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static uint8_t	check_the_table_one_last_time_befor_dinner_begin(
		const struct s_the_table *table)
{
	if (table->n_philo == 0)
	{
		printf ("%sError:%s It must be at least one=1 guest (philosopher)\n",
			RED, EOC);
		return (1);
	}
	else if (table->n_philo > 200)
	{
		printf ("%sWarrning:%s such number of philosopher", YELLOW, EOC);
		printf (" in the same room may results in unspecified behavior\n");
	}
	if (table->time_die == 0)
		return (printf ("%sError:%s Everyone should die at one %stime%s\n",
				   	RED, EOC, BLUE, EOC));
	if (table->time_eat == 0)
		return (printf ("%sError:%s Every living being must eat at one time\n",
				   RED, EOC));
	if (table->time_sleep == 0)
		return (printf ("%sError:%s Philosopher must eat at one %stime%s\n",
				RED, EOC, BLUE, EOC));
	if (table->limited_meals && table->eat_limit == 0)
		return (printf ("%sError:%s We need to serve at least one %smeal%s\n",
				RED, EOC, GREEN, EOC));
	return (0);
}

static void	set_the_cultery(struct s_the_table *table, char **av, const int ac)
{
	table->n_philo = ft_atoi(av[1]);
	table->time_die = ft_atoi(av[2]);
	table->time_eat = ft_atoi(av[3]);
	table->time_sleep = ft_atoi(av[4]);
	table->limited_meals = (ac == 6);
	if (table->limited_meals)
		table->eat_limit = ft_atoi(av[5]);
	else
		table->eat_limit = 0;
	table->someone_die = 0;
	table->finished_meal = 0;
}

uint8_t	dress_table(struct s_the_table *table, t_philo **philosopher,
		char **av, const int ac)
{
	uint32_t	i;

	set_the_cultery(table, av, ac);
	table->philos = malloc(sizeof(pthread_t) * table->n_philo);
	*philosopher = malloc(sizeof(t_philo) * table->n_philo);
	table->forks = malloc(sizeof(pthread_mutex_t) * (table->n_philo));
	if (!table->philos || !table->forks || !(*philosopher))
		return (printf("%sError:%s memory allocation have failed", RED, EOC));
	i = 0;
	while (i < table->n_philo)
	{
		pthread_mutex_init(table->forks + i, NULL);
		(*philosopher)[i].table = table;
		(*philosopher)[i].meals_eated = 0;
		(*philosopher)[i].name = i + 1;
		(*philosopher)[i].has_finished = 0;
		++i;
	}
	pthread_mutex_init(&table->display, NULL);
	return (check_the_table_one_last_time_befor_dinner_begin(table));
}
