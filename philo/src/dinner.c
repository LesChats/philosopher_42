/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 17:54:21 by abaudot           #+#    #+#             */
/*   Updated: 2022/02/10 22:34:54 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	*dinner(void *phi)
{
	t_philo *const	philo = phi;
	uint8_t const	limited_meals = philo->perspective->limited_meals;

	philo->last_meal = get_timestamp();
	while (1)
	{
		pthread_mutex_lock(&philo->perspective->display);
		if (philo->perspective->someone_die)
		{
			pthread_mutex_unlock(&philo->perspective->display);
			break ;
		}
		pthread_mutex_unlock(&philo->perspective->display);
		take_forks(philo);
		eat_(philo);
		if (limited_meals)
			if (philo->meals_eated >= philo->perspective->eat_limit)
				break ;
		sleep_(philo);
		think_(philo);
	}
	pthread_mutex_lock(&philo->perspective->display);
	++philo->perspective->finished_meal;
	pthread_mutex_unlock(&philo->perspective->display);
	return (NULL);
}
