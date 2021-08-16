/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:07:26 by abaudot           #+#    #+#             */
/*   Updated: 2021/08/14 23:56:53 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

void	take_forks(const t_philo *philo)
{
	if (philo->is_dead)
		return ;
	sem_wait(philo->forks);
	sem_wait(philo->forks);
	sem_wait(philo->display);
	annonce(philo, FORK);
	annonce(philo, FORK);
	sem_post(philo->display);
}

void	eat_(t_philo *philo)
{
	if (philo->is_dead)
		return ;
	sem_wait(philo->display);
	annonce(philo, EAT);
	philo->last_meal = get_time(&philo->table->time_start);
	++philo->meals_eated;
	sem_post(philo->display);
	usleep(philo->table->time_eat);
	sem_post(philo->forks);
	sem_post(philo->forks);
}

void	sleep_(const t_philo *philo)
{
	if (philo->is_dead)
		return ;
	sem_wait(philo->display);
	annonce(philo, SLEEP);
	sem_post(philo->display);
	usleep(philo->table->time_sleep);
}

void	think_(const t_philo *philo)
{
	if (philo->is_dead)
		return ;
	sem_wait(philo->display);
	annonce(philo, THINK);
	sem_post(philo->display);
}

void	*death_prediction(void *phi)
{
	t_philo *const	philo = phi;
	uint32_t		time_;

	if (philo->is_dead)
		return (NULL);
	usleep(philo->table->time_die * 1000);
	if (philo->is_dead)
		return (NULL);
	time_ = get_time(&philo->table->time_start);
	if (time_ >= philo->last_meal + philo->table->time_die)
	{
		if (philo->is_dead)
			return (NULL);
		philo->is_dead = 1 + (!philo->table->limited_meals
				|| philo->meals_eated < philo->table->eat_limit);
		if (philo->is_dead == 2)
		{
			sem_wait(philo->display);
			annonce(philo, DEATH);
		}
	}
	return (NULL);
}
