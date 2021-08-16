/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:18:19 by abaudot           #+#    #+#             */
/*   Updated: 2021/08/16 13:33:33 by abaudot          ###   ########.fr       */
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

static void	monitor(t_philo *philo)
{
	uint32_t	i;
	uint32_t	finished_meals;
	int			status;

	status = 0;
	finished_meals = 0;
	i = 0;
	while (1)
	{
		waitpid(philo[i].pid, &status, WNOHANG);
		if (WEXITSTATUS(status) == 2)
			return (kill_all(philo));
		if (WEXITSTATUS(status) == 1)
		{
			if (++finished_meals == philo->table->n_philo)
			{
				printf ("\n%s*\t%d\t%sAll meals have been eated *%s\n",
					PURPLE, get_time(&philo->table->time_start), GREEN, EOC);
				return (kill_all(philo));
			}
		}
		++i;
		i *= (i != philo->table->n_philo);
	}
}

static int	dinner(t_philo *philo)
{
	pthread_t	death_oracle;

	while (!philo->is_dead && (!philo->table->limited_meals
			|| philo->meals_eated < philo->table->eat_limit))
	{
		pthread_create(&death_oracle, NULL, &death_prediction, philo);
		pthread_detach(death_oracle);
		take_forks(philo);
		eat_(philo);
		sleep_(philo);
		think_(philo);
	}
	return (philo->is_dead + !philo->is_dead);
}

static void	start_dinner(t_philo *philos)
{
	uint32_t	i;
	int			tmp;

	i = 0;
	gettimeofday(&(philos->table->time_start), NULL);
	while (i < philos->table->n_philo)
	{
		philos[i].pid = fork();
		if (philos[i].pid == 0)
		{
			tmp = dinner(philos + i);
			usleep(philos->table->time_die * 1000);
			sem_close(philos->forks);
			sem_close(philos->display);
			free(philos);
			exit(tmp);
		}
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
