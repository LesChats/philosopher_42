/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:07:26 by abaudot           #+#    #+#             */
/*   Updated: 2022/01/31 18:51:38 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	take_forks(const t_philo *philo)
{
	pthread_mutex_lock(philo->perspective->forks + (philo->name - 1));
	pthread_mutex_lock(philo->perspective->forks
		+ (philo->name * !(philo->name == philo->perspective->n_philo)));
	pthread_mutex_lock(&philo->perspective->display);
	annonce(philo, FORK);
	annonce(philo, FORK);
	pthread_mutex_unlock(&philo->perspective->display);
}

void	eat_(t_philo *philo)
{
	pthread_mutex_lock(&philo->perspective->display);
	++philo->meals_eated;
	philo->last_meal = get_timestamp();
	annonce(philo, EAT);
	pthread_mutex_unlock(&philo->perspective->display);
	if (!philo->perspective->someone_die)
		ft_usleep(philo->perspective->time_eat);
	pthread_mutex_unlock(philo->perspective->forks + (philo->name - 1));
	pthread_mutex_unlock(philo->perspective->forks
		+ (philo->name * !(philo->name == philo->perspective->n_philo)));
}

void	sleep_(const t_philo *philo)
{
	pthread_mutex_lock(&philo->perspective->display);
	annonce(philo, SLEEP);
	pthread_mutex_unlock(&philo->perspective->display);
	if (!philo->perspective->someone_die)
		ft_usleep(philo->perspective->time_sleep);
}

void	think_(const t_philo *philo)
{
	pthread_mutex_lock(&philo->perspective->display);
	annonce(philo, THINK);
	pthread_mutex_unlock(&philo->perspective->display);
}

void	*death_prediction(void *philo)
{
	t_philo *const	p = philo;

	while (!p->perspective->someone_die)
	{
		if (p->perspective->limited_meals
			&& p->meals_eated >= p->perspective->eat_limit)
		{
			p->has_finished = 1;
			return (NULL);
		}
		pthread_mutex_lock(&p->perspective->display);
		if (get_timestamp() - p->last_meal >= p->perspective->time_die)
		{
			annonce(p, DEATH);
			p->perspective->someone_die = 1;
			pthread_mutex_unlock(p->perspective->forks);
			pthread_mutex_unlock(p->perspective->forks
				+ (p->name * !(p->name == p->perspective->n_philo)));
			pthread_mutex_unlock(&p->perspective->display);
			return (NULL);
		}
		usleep(1);
		pthread_mutex_unlock(&p->perspective->display);
	}
	return (NULL);
}
