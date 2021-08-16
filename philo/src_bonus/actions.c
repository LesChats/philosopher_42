/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:07:26 by abaudot           #+#    #+#             */
/*   Updated: 2021/08/14 00:04:15 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

void	take_forks(const t_philo *philo)
{
	sem_wait(philo->forks);
	sem_wait(philo->forks);
	if (philo->is_dead)
		return ;
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
	++philo->meals_eated;
	sem_post(philo->display);
	philo->last_meal = get_time(&philo->table->time_start);
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

	pthread_detach(pthread_self());
	usleep(philo->table->time_die * 1000);
	time_ = get_time(&philo->table->time_start);
	if (time_ - philo->last_meal >= philo->table->time_die)
	{
		philo->is_dead = !(philo->table->limited_meals
				&& philo->meals_eated >= philo->table->eat_limit);
		if (philo->is_dead)
		{
//			printf("ded\n");
			sem_wait(philo->display);
			annonce(philo, DEATH);
		}
//		printf ("okdfdsfsdf\n");
	}
	return (NULL);
}
