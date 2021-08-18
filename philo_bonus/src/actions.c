/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:07:26 by abaudot           #+#    #+#             */
/*   Updated: 2021/08/18 17:37:01 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

void	take_forks(const t_philo *philo)
{
	sem_wait(philo->forks);
	sem_wait(philo->forks);
	sem_wait(philo->display);
	annonce(philo, FORK);
	annonce(philo, FORK);
	sem_post(philo->display);
}

void	eat_(t_philo *philo)
{
	sem_wait(philo->display);
	annonce(philo, EAT);
	philo->last_meal = get_time(&philo->table->time_start);
	sem_post(philo->eat_sem);
	sem_post(philo->display);
	usleep(philo->table->time_eat);
	sem_post(philo->forks);
	sem_post(philo->forks);
}

void	sleep_(const t_philo *philo)
{
	sem_wait(philo->display);
	annonce(philo, SLEEP);
	sem_post(philo->display);
	usleep(philo->table->time_sleep);
}

void	think_(const t_philo *philo)
{
	sem_wait(philo->display);
	annonce(philo, THINK);
	sem_post(philo->display);
}

void	*death_prediction(void *phi)
{
	t_philo *const	philo = phi;
	uint32_t		time_;

	while (1)
	{
		sem_wait(philo->display);
		time_ = get_time(&philo->table->time_start);
		if (time_ > philo->last_meal + philo->table->time_die)
		{
			annonce_death(philo);
			sem_post(philo->kill_table);
			return (NULL);
		}
		sem_post(philo->display);
		usleep(1000);
	}
	return (NULL);
}
