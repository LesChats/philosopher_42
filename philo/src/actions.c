/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:07:26 by abaudot           #+#    #+#             */
/*   Updated: 2022/02/09 17:11:02 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	monitor(struct s_the_table *table, t_philo *philo)
{
	long		finish_time;
	uint32_t	i;

	while (table->finished_meal != table->n_philo)
	{
		i = 0;
		pthread_mutex_lock(&table->display);
		finish_time = get_timestamp();
		while (i < table->n_philo)
		{
			if (finish_time - philo[i].last_meal >= table->time_die)
			{
				annonce(philo + i, DEATH);
				table->someone_die = 1;
				pthread_mutex_unlock(table->forks + i);
				pthread_mutex_unlock(table->forks + (i + 1)
					* (philo[i].name != table->n_philo));
				pthread_mutex_unlock(&table->display);
				return ;
			}
			++i;
		}
		pthread_mutex_unlock(&table->display);
		usleep(1000);
	}
}

void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->perspective->forks + (philo->name - 1));
	pthread_mutex_lock(philo->perspective->forks
		+ (philo->name * !(philo->name == philo->perspective->n_philo)));
	pthread_mutex_lock(&philo->perspective->display);
	philo->offset = get_timestamp();
	annonce(philo, FORK);
	annonce(philo, FORK);
	pthread_mutex_unlock(&philo->perspective->display);
}

void	eat_(t_philo *philo)
{
	uint32_t	time;

	pthread_mutex_lock(&philo->perspective->display);
	annonce(philo, EAT);
	++philo->meals_eated;
	philo->last_meal = get_timestamp();
	pthread_mutex_unlock(&philo->perspective->display);
	time = philo->perspective->time_eat - (get_timestamp() - philo->offset);
	ft_usleep(time * (!philo->perspective->someone_die));
	philo->offset = get_timestamp();
	pthread_mutex_unlock(philo->perspective->forks + (philo->name - 1));
	pthread_mutex_unlock(philo->perspective->forks
		+ (philo->name * !(philo->name == philo->perspective->n_philo)));
}

void	sleep_(t_philo *philo)
{
	uint32_t	time;

	pthread_mutex_lock(&philo->perspective->display);
	annonce(philo, SLEEP);
	pthread_mutex_unlock(&philo->perspective->display);
	time = philo->perspective->time_sleep - (get_timestamp() - philo->offset);
	ft_usleep(time * !philo->perspective->someone_die);
	philo->offset = get_timestamp();
}

void	think_(t_philo *philo)
{
	pthread_mutex_lock(&philo->perspective->display);
	annonce(philo, THINK);
	pthread_mutex_unlock(&philo->perspective->display);
}
