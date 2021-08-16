/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:18:19 by abaudot           #+#    #+#             */
/*   Updated: 2021/08/14 23:42:08 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static void	monitor(struct s_the_table *table)
{
	while (table->finished_meal != table->n_philo)
		usleep(1000);
	if (table->limited_meals && table->someone_die != 1)
		printf ("\n%s*\t%d\t%sAll meals have been eated *%s\n",
			PURPLE, get_time(&table->time_start), GREEN, EOC);
	usleep(1000 * table->time_die);
}

static void	*dinner(void *phi)
{
	t_philo *const	philo = phi;
	pthread_t		death_oracle;

	while (!(philo->table->someone_die) && (!philo->table->limited_meals
			|| philo->meals_eated < philo->table->eat_limit))
	{
		pthread_create(&death_oracle, NULL, &death_prediction, phi);
		pthread_detach(death_oracle);
		take_forks(philo);
		eat_(philo);
		sleep_(philo);
		think_(philo);
	}
	++philo->table->finished_meal;
	return (NULL);
}	

static char	start_dinner(t_philo *philos)
{
	uint32_t	i;

	gettimeofday(&(philos->table->time_start), NULL);
	i = 0;
	while (i < philos->table->n_philo)
	{
		if (pthread_create(philos->table->philos + i,
				NULL, &dinner, philos + i))
			return (printf ("%sError:%s pthread_create fail\n", RED, EOC));
		pthread_detach(philos->table->philos[i]);
		usleep(5);
		++i;
	}
	return (0);
}

static int	clean_the_table_and_send_the_philosophers_home(
		struct s_the_table *table, t_philo **philos)
{
	uint32_t	i;

	pthread_mutex_destroy(&table->display);
	i = 0;
	if (table->forks)
	{
		while (i < table->n_philo)
		{
			pthread_mutex_destroy(table->forks + i);
			++i;
		}
		free(table->forks);
	}
	if (table->philos)
		free(table->philos);
	if (*philos)
		free(*philos);
	return (0);
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
		printf (" [num of meal]\n");
		return (1);
	}
	if (dress_table(&table, &philos, av, ac))
		return (clean_the_table_and_send_the_philosophers_home(
				&table, &philos));
	if (start_dinner(philos))
		return (clean_the_table_and_send_the_philosophers_home(
				&table, &philos));
	monitor(&table);
	return (clean_the_table_and_send_the_philosophers_home(
			&table, &philos));
}
