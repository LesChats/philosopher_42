/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:07:26 by abaudot           #+#    #+#             */
/*   Updated: 2021/08/23 17:23:45 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	take_forks(const t_philo *philo)
{
	pthread_mutex_lock(philo->table->forks + (philo->name - 1));
	pthread_mutex_lock(philo->table->forks
		+ (philo->name * !(philo->name == philo->table->n_philo)));
	pthread_mutex_lock(&philo->table->display);
	annonce(philo, FORK);
	annonce(philo, FORK);
	pthread_mutex_unlock(&philo->table->display);
}

void	eat_(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->display);
	++philo->meals_eated;
	philo->last_meal = get_timestamp();
	annonce(philo, EAT);
	pthread_mutex_unlock(&philo->table->display);
	if (!philo->table->someone_die)
		ft_usleep(philo->table->time_eat);
	pthread_mutex_unlock(philo->table->forks + (philo->name - 1));
	pthread_mutex_unlock(philo->table->forks
		+ (philo->name * !(philo->name == philo->table->n_philo)));
}

void	sleep_(const t_philo *philo)
{
	pthread_mutex_lock(&philo->table->display);
	annonce(philo, SLEEP);
	pthread_mutex_unlock(&philo->table->display);
	if (!philo->table->someone_die)
		ft_usleep(philo->table->time_sleep);
}

void	think_(const t_philo *philo)
{
	pthread_mutex_lock(&philo->table->display);
	annonce(philo, THINK);
	pthread_mutex_unlock(&philo->table->display);
}

void	*death_prediction(void *phi)
{
	t_philo *const	p = phi;

	while (!p->table->someone_die)
	{
		if (p->table->limited_meals && p->meals_eated >= p->table->eat_limit)
		{
			p->has_finished = 1;
			return (NULL);
		}
		pthread_mutex_lock(&p->table->display);
		if (get_timestamp() - p->last_meal >= p->table->time_die)
		{
			annonce(p, DEATH);
			p->table->someone_die = 1;
			pthread_mutex_unlock(p->table->forks);
			pthread_mutex_unlock(p->table->forks
				+ (p->name * !(p->name == p->table->n_philo)));
			pthread_mutex_unlock(&p->table->display);
			return (NULL);
		}
		pthread_mutex_unlock(&p->table->display);
		usleep(100);
	}
	return (NULL);
}
