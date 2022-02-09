/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/18 17:38:35 by abaudot           #+#    #+#             */
/*   Updated: 2022/02/09 16:17:01 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosopher_bonus.h>

static void	kill_all(t_philo *philo)
{
	uint32_t	i;

	i = 0;
	while (i < philo->table->n_philo)
	{
		kill((philo + i)->pid, SIGKILL);
		++i;
	}
	usleep(100);
}

static void	*monitor_count(void *phi)
{
	t_philo *const				philo = phi;
	const struct s_the_table	*table = philo->table;
	uint32_t					total;
	uint32_t					i;

	total = 0;
	while (total <= table->eat_limit)
	{
		i = 0;
		while (i < table->n_philo)
			sem_wait(philo[i++].eat_sem);
		++total;
	}
	sem_wait(philo->display);
	write (1, ALL, LEN * (table->eat_limit > 0));
	sem_post(philo->kill_table);
	return (NULL);
}

static void	monitor(t_philo *philo)
{
	pthread_t	eat_count;

	sem_wait(philo->kill_table);
	if (philo->table->eat_limit)
	{
		pthread_create(&eat_count, NULL, &monitor_count, philo);
		pthread_detach(eat_count);
	}
	sem_wait(philo->kill_table);
	kill_all(philo);
	philo->table->eat_limit = 0;
}

static void	dinner(t_philo *philo)
{
	pthread_t	death_oracle;

	philo->last_meal = get_timestamp();
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

void	start_dinner(t_philo *philos)
{
	uint32_t	i;

	i = 0;
	philos->table->time_start = get_timestamp();
	while (i < philos->table->n_philo)
	{
		philos[i].pid = fork();
		if (philos[i].pid == 0)
			dinner(philos + i);
		usleep(30);
		++i;
	}
	monitor(philos);
}
