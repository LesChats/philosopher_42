/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:07:26 by abaudot           #+#    #+#             */
/*   Updated: 2021/08/14 23:45:35 by abaudot          ###   ########.fr       */
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
	philo->last_meal = get_time(&philo->table->time_start);
	annonce(philo, EAT);
	pthread_mutex_unlock(&philo->table->display);
	if (!philo->table->someone_die)
		usleep(philo->table->time_eat);
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
		usleep(philo->table->time_sleep);
}

void	think_(const t_philo *philo)
{
	pthread_mutex_lock(&philo->table->display);
	annonce(philo, THINK);
	pthread_mutex_unlock(&philo->table->display);
}

void	*death_prediction(void *phi)
{
	t_philo *const	philo = phi;
	uint32_t		time_;

	if (philo->table->someone_die)
		return (NULL);
	usleep(philo->table->time_die * 1000);
	time_ = get_time(&philo->table->time_start);
	if (time_ >= philo->last_meal + philo->table->time_die)
	{
		if (!philo->table->limited_meals
			|| philo->meals_eated < philo->table->eat_limit)
		{
			annonce(philo, DEATH);
			philo->table->someone_die = 1;
		}
		else
			philo->table->someone_die = 2;
		pthread_mutex_unlock(philo->table->forks);
		pthread_mutex_unlock(philo->table->forks
			+ (philo->name * !(philo->name == philo->table->n_philo)));
	}
	return (NULL);
}
