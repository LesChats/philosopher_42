/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:18:19 by abaudot           #+#    #+#             */
/*   Updated: 2021/08/17 17:13:24 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

static void	kill_all(t_philo *philo)
{
	while (philo->name)
	{
		kill(philo->pid, SIGKILL);
		++philo;
	}
}

static void	*monitor_count(void *phi)
{
	t_philo *const	philo = phi;
	const struct s_the_table *table = philo->table;
	uint32_t		total = 0;
	uint32_t		i;

	while (total <= table->eat_limit)
	{
		i = 0;
		while (i < table->n_philo)
			sem_wait(philo[i++].eat_sem);
		++total;
	}
	sem_wait(philo->display);
	write (1, ALL, LEN);
	sem_post(philo->kill_table);
	return (NULL);
}
	

static void	monitor(t_philo *philo)
{
	pthread_t	eat_count;

	sem_wait(philo->kill_table);
	if (philo->table->limited_meals)
	{
		pthread_create(&eat_count, NULL, &monitor_count, philo);
		pthread_detach(eat_count);
	}
	sem_wait(philo->kill_table);
	kill_all(philo);
}

static void	dinner(t_philo *philo)
{
	pthread_t	death_oracle;

	pthread_create(&death_oracle, NULL, &death_prediction, philo);
	pthread_detach(death_oracle);
	while (1)
	{
		take_forks(philo);
		eat_(philo);
		sleep_(philo);
		think_(philo);
	}
}

static void	start_dinner(t_philo *philos)
{
	uint32_t	i;

	i = 0;
	gettimeofday(&(philos->table->time_start), NULL);
	while (i < philos->table->n_philo)
	{
		philos[i].pid = fork();
		if (philos[i].pid == 0)
			dinner(philos + i);
		usleep(30);
		++i;
	}
}

int	main(int ac, char **av)
{
	struct s_the_table	table;
	t_philo				*philos;

	if (ac != 5 && ac != 6)
	{
		printf ("%sError:%s Wrong usage\n", RED, EOC);
		printf ("%sUsage%s ./philo [num on philo] [time to die]", YELLOW, EOC);
		printf (" [time to eat] [time to sleep] %soptional:%s", BLUE, EOC);
		return (printf (" [num of meal]\n"));
	}
	if (dress_table(&table, &philos, av, ac))
	{
		sem_close(philos->display);
		sem_close(philos->forks);
		free(philos);
		return (1);
	}
	start_dinner(philos);
	monitor(philos);
	sem_close(philos->display);
	sem_close(philos->forks);
	free(philos);
	return (0);
}
