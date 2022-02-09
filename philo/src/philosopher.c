/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:18:19 by abaudot           #+#    #+#             */
/*   Updated: 2022/02/09 21:20:10 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static void	*dinner(void *phi)
{
	t_philo *const	philo = phi;
	uint8_t const	limited_meals = philo->perspective->limited_meals;

	philo->last_meal = get_timestamp();
	while (1)
	{
		pthread_mutex_lock(&philo->perspective->display);
		if (philo->perspective->someone_die)
		{
			pthread_mutex_unlock(&philo->perspective->display);
			break ;
		}
		pthread_mutex_unlock(&philo->perspective->display);
		take_forks(philo);
		eat_(philo);
		if (limited_meals)
		{
			if (philo->meals_eated >= philo->perspective->eat_limit)
				break ;
		}
		sleep_(philo);
		think_(philo);
	}
	pthread_mutex_lock(&philo->perspective->display);
	++philo->perspective->finished_meal;
	pthread_mutex_unlock(&philo->perspective->display);
	return (NULL);
}

static uint32_t	lauch_philo(t_philo *philos, struct s_the_table const *table,
		uint32_t start)
{
	uint32_t	i;

	i = start;
	while (i < table->n_philo)
	{
		if (pthread_create(table->philos + i, NULL, &dinner, philos + i))
			return (printf ("%sError:%s pthread_create fail\n", RED, EOC));
		i += 2;
	}
	return (0);
}

static char	start_dinner(t_philo *philos, struct s_the_table *table)
{
	uint32_t	i;

	pthread_mutex_lock(&table->display);
	if (lauch_philo(philos, table, 0))
		return (1);
	table->time_start = get_timestamp();
	pthread_mutex_unlock(&table->display);
	ft_usleep(table->time_eat);
	if (lauch_philo(philos, table, 1))
		return (1);
	usleep(2000);
	monitor(table, philos);
	i = 0;
	while (i < table->n_philo)
		pthread_join(table->philos[i++], NULL);
	return (0);
}

static int	clean_the_table_and_send_the_philosophers_home(
		struct s_the_table *table, t_philo **philos)
{
	uint32_t	i;

	pthread_mutex_destroy(&table->display);
	if (table->forks)
	{
		i = 0;
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
	if (!table.someone_die)
		printf ("\n%s*\t%ld\t%sAll meals have been eated *%s\n",
			PURPLE, get_timestamp() - table.time_start, GREEN, EOC);
	return (clean_the_table_and_send_the_philosophers_home(
			&table, &philos));
}
