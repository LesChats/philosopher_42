/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo_table.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:07:40 by abaudot           #+#    #+#             */
/*   Updated: 2022/01/31 16:16:40 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

static uint8_t	check_the_table_one_last_time_befor_dinner_begin(
		const struct s_the_table *table)
{
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
	if (table->n_philo > 200)
	{
		printf ("%sWarrning:%s such number of philosopher", YELLOW, EOC);
		printf (" in the same room may results in unspecified behavior\n");
		usleep(3000);
	}
	return (0);
}

static void	set_the_cultery(struct s_the_table *table, char **av, const int ac)
{
	sem_unlink("forks");
	sem_unlink("display");
	sem_unlink("kill_table");
	table->n_philo = ft_atoi(av[1]);
	table->time_die = ft_atoi(av[2]);
	table->time_eat = ft_atoi(av[3]);
	table->time_sleep = ft_atoi(av[4]);
	table->limited_meals = (ac == 6);
	if (table->limited_meals)
		table->eat_limit = ft_atoi(av[5]);
	else
		table->eat_limit = 0;
}

static uint8_t	rewiew_guest_list(t_philo *philos, uint32_t n_philo)
{
	char		sem_name[22];
	sem_t		**eat_sem;

	memset(sem_name, ' ', 22);
	sem_name[21] = 0;
	eat_sem = malloc(sizeof(sem_t *) * n_philo);
	if (!eat_sem)
		return (printf("%sError:%s memory allocation have failed", RED, EOC));
	ft_buffnbr(1, sem_name, 21);
	sem_unlink(sem_name);
	philos->eat_sem = sem_open(sem_name, O_CREAT | O_EXCL, S_IRWXU, 1);
	while (--n_philo)
	{
		ft_buffnbr(n_philo + 1, sem_name, 21);
		sem_unlink(sem_name);
		philos[n_philo].eat_sem = sem_open(sem_name,
				O_CREAT | O_EXCL, S_IRWXU, 1);
		philos[n_philo].table = philos->table;
		philos[n_philo].name = n_philo + 1;
		philos[n_philo].forks = philos->forks;
		philos[n_philo].display = philos->display;
		philos[n_philo].kill_table = philos->kill_table;
	}
	free(eat_sem);
	return (0);
}

uint8_t	dress_table(struct s_the_table *table, t_philo **philosopher,
		char **av, const int ac)
{
	set_the_cultery(table, av, ac);
	*philosopher = malloc(sizeof(t_philo) * (table->n_philo));
	if (!*philosopher)
		return (printf("%sError:%s memory allocation have failed", RED, EOC));
	if (table->n_philo == 0)
		return (printf ("%sError:%s It must be at least one=1 guest\n",
				RED, EOC));
	(*philosopher)->table = table;
	(*philosopher)->name = 1;
	(*philosopher)->last_meal = 0;
	(*philosopher)->forks = sem_open("forks",
			O_CREAT | O_EXCL, S_IRWXU, table->n_philo);
	(*philosopher)->display = sem_open("display",
			O_CREAT | O_EXCL, S_IRWXU, 1);
	(*philosopher)->kill_table = sem_open("kill_table",
			O_CREAT | O_EXCL, S_IRWXU, 1);
	if (rewiew_guest_list(*philosopher, table->n_philo))
		return (1);
	return (check_the_table_one_last_time_befor_dinner_begin(table));
}
