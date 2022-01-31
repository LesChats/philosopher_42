/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:18:19 by abaudot           #+#    #+#             */
/*   Updated: 2022/01/31 15:27:24 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static void	monitor(struct s_the_table *table)
{
	while (table->finished_meal != table->n_philo)
		usleep(1000);
	if (!table->someone_die)
		printf ("\n%s*\t%ld\t%sAll meals have been eated *%s\n",
			PURPLE, get_timestamp() - table->time_start, GREEN, EOC);
}

static void	*dinner(void *phi)
{
	t_philo *const	philo = phi;
	pthread_t		death_oracle;

	philo->last_meal = get_timestamp();
	pthread_create(&death_oracle, NULL, &death_prediction, phi);
	pthread_detach(death_oracle);
	while (!philo->perspective->someone_die && !philo->has_finished)
	{
		take_forks(philo);
		eat_(philo);
		sleep_(philo);
		think_(philo);
	}
	++philo->perspective->finished_meal;
	return (NULL);
}

static char	start_dinner(t_philo *philos, struct s_the_table *table)
{
	uint32_t	i;

	table->time_start = get_timestamp();
	i = 0;
	while (i < table->n_philo)
	{
		if (pthread_create(table->philos + i,
				NULL, &dinner, philos + i))
			return (printf ("%sError:%s pthread_create fail\n", RED, EOC));
		pthread_detach(table->philos[i]);
		usleep(10);
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
	if (start_dinner(philos, &table))
		return (clean_the_table_and_send_the_philosophers_home(
				&table, &philos));
	monitor(&table);
	return (clean_the_table_and_send_the_philosophers_home(
			&table, &philos));
}
