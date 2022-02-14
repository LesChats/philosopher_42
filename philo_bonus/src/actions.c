/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:07:26 by abaudot           #+#    #+#             */
/*   Updated: 2022/02/14 13:24:57 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

void	take_forks(t_philo *philo)
{
	sem_wait(philo->forks);
	sem_wait(philo->display);
	annonce(philo, FORK);
	sem_post(philo->display);
	sem_wait(philo->forks);
	philo->offset = get_timestamp();
	annonce(philo, FORK);
	sem_post(philo->display);
}

void	eat_(t_philo *philo)
{
	uint32_t	time;

	sem_wait(philo->display);
	annonce(philo, EAT);
	philo->last_meal = get_timestamp();
	sem_post(philo->eat_sem);
	sem_post(philo->display);
	time = philo->table->time_eat - (get_timestamp() - philo->offset);
	ft_usleep(time);
	philo->offset = get_timestamp();
	sem_post(philo->forks);
	sem_post(philo->forks);
}

void	sleep_(t_philo *philo)
{
	uint32_t	time;

	sem_wait(philo->display);
	annonce(philo, SLEEP);
	sem_post(philo->display);
	time = philo->table->time_sleep - (get_timestamp() - philo->offset);
	ft_usleep(time);
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

	while (1)
	{
		sem_wait(philo->display);
		if (get_timestamp() >= philo->last_meal + philo->table->time_die)
		{
			annonce(philo, DEATH);
			sem_post(philo->kill_table);
			return (NULL);
		}
		sem_post(philo->display);
		usleep(philo->table->time_die >> 1);
	}
	return (NULL);
}
