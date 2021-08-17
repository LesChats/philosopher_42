/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo_table.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:07:40 by abaudot           #+#    #+#             */
/*   Updated: 2021/08/17 16:20:03 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

static uint8_t	check_the_table_one_last_time_befor_dinner_begin(
		const struct s_the_table *table)
{
	if (table->n_philo == 0)
		return (printf ("%sError:%s It must be at least one=1 guest\n",
				RED, EOC));
	if (table->n_philo > 200)
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
	if (table->n_philo == 1)
		return (one_philo(table));
	return (0);
}

static void	set_the_cultery(struct s_the_table *table, char **av, const int ac)
{
	sem_unlink("forks");
	sem_unlink("display");
	sem_unlink("kill_table");
	table->n_philo = ft_atoi(av[1]);
	table->time_die = ft_atoi(av[2]);
	table->time_eat = ft_atoi(av[3]) * 1000;
	table->time_sleep = ft_atoi(av[4]) * 1000;
	table->limited_meals = (ac == 6);
	if (table->limited_meals)
		table->eat_limit = ft_atoi(av[5]);
	else
		table->eat_limit = 0;
}

uint8_t	dress_table(struct s_the_table *table, t_philo **philosopher,
		char **av, const int ac)
{
	uint32_t	i;
	sem_t		*forks;
	sem_t		*display;
	sem_t		*kill_table;
	sem_t		**eat_sem;
	static char	sem_name[64] = "eat                     ";


	set_the_cultery(table, av, ac);
	eat_sem = malloc(sizeof(sem_t *) * table->n_philo);
	*philosopher = malloc(sizeof(t_philo) * (table->n_philo + 1));
	if (!*philosopher)
		return (printf("%sError:%s memory allocation have failed", RED, EOC));
	forks = sem_open("forks", O_CREAT | O_EXCL, S_IRWXU, table->n_philo);
	display = sem_open("display", O_CREAT | O_EXCL, S_IRWXU, 1);
	kill_table = sem_open("kill_table", O_CREAT | O_EXCL, S_IRWXU, 1);
	i = 0;
	while (i < table->n_philo)
	{
		ft_buffnbr(i + 1, sem_name + 4, 20);
		sem_unlink(sem_name);
		(*philosopher)[i].eat_sem = sem_open(sem_name, O_CREAT | O_EXCL, S_IRWXU, 1);
		(*philosopher)[i].table = table;
		(*philosopher)[i].last_meal = 0;
//		(*philosopher)[i].meals_eated = 0;
		(*philosopher)[i].name = i + 1;
//		(*philosopher)[i].is_dead = 0;
		(*philosopher)[i].forks = forks;
		(*philosopher)[i].display = display;
		(*philosopher)[i].kill_table = kill_table;
		++i;
	}
	(*philosopher)[table->n_philo].name = 0;
	return (check_the_table_one_last_time_befor_dinner_begin(table));
}
