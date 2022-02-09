/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:07:26 by abaudot           #+#    #+#             */
/*   Updated: 2022/02/09 21:25:22 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	monitor(struct s_the_table *table, t_philo *philo)
{
	long		finish_time;
	uint32_t	i;

	while (1)
	{
		i = 0;
		pthread_mutex_lock(&table->display);
		if (table->finished_meal == table->n_philo)
		{
			pthread_mutex_unlock(&table->display);
			return ;
		}
		finish_time = get_timestamp();
		while (i < table->n_philo)
		{
			if (finish_time - philo[i].last_meal >= table->time_die)
			{
				annonce(philo + i, DEATH);
				table->someone_die = 1;
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
	uint32_t	tmp;

	tmp = philo->name - (philo->name & 1);
	tmp *= !(tmp == philo->perspective->n_philo);
	pthread_mutex_lock(philo->perspective->forks + tmp);
	tmp = philo->name - !(philo->name & 1);
	tmp *= !(tmp == philo->perspective->n_philo);
	pthread_mutex_lock(philo->perspective->forks + tmp);
	philo->offset = get_timestamp();
	pthread_mutex_lock(&philo->perspective->display);
	annonce(philo, FORK);
	annonce(philo, FORK);
	pthread_mutex_unlock(&philo->perspective->display);
}

void	eat_(t_philo *philo)
{
	uint32_t	time;
	uint8_t		someone_die;

	pthread_mutex_lock(&philo->perspective->display);
	someone_die = philo->perspective->someone_die;
	annonce(philo, EAT);
	++philo->meals_eated;
	philo->last_meal = get_timestamp();
	pthread_mutex_unlock(&philo->perspective->display);


	time = philo->perspective->time_eat - (get_timestamp() - philo->offset);
	ft_usleep(time * (!someone_die));
	philo->offset = get_timestamp();
	pthread_mutex_unlock(philo->perspective->forks + (philo->name - 1));
	pthread_mutex_unlock(philo->perspective->forks
		+ (philo->name * !(philo->name == philo->perspective->n_philo)));
}

void	sleep_(t_philo *philo)
{
	uint32_t	time;
	uint8_t		someone_die;

	pthread_mutex_lock(&philo->perspective->display);
	someone_die = philo->perspective->someone_die;
	annonce(philo, SLEEP);
	pthread_mutex_unlock(&philo->perspective->display);

	time = philo->perspective->time_sleep - (get_timestamp() - philo->offset);
	ft_usleep(time * !someone_die);
	philo->offset = get_timestamp();
}

void	think_(t_philo *philo)
{
	pthread_mutex_lock(&philo->perspective->display);
	annonce(philo, THINK);
	pthread_mutex_unlock(&philo->perspective->display);
}
