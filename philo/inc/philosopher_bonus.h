/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_bonus.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaudot <abaudot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 21:18:03 by abaudot           #+#    #+#             */
/*   Updated: 2021/08/13 21:36:34 by abaudot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <sys/time.h>
# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <stdint.h>
# include <sys/wait.h>
# include <signal.h>
# include <semaphore.h>
# include <fcntl.h>

# define	BLACK	"\033[1;30m"
# define	RED		"\033[1;31m"
# define	GREEN	"\033[1;32m"
# define	YELLOW	"\033[1;33m"
# define	BLUE	"\033[1;34m"
# define	CYAN	"\033[1;35m"
# define	PURPLE	"\033[1;36m"
# define	WHITE	"\033[1;37m"
# define	EOC		"\033[0;0m"

# define	DEATH	"\033[1;31m died            \033[0;0m\n"
# define	SLEEP	"\033[1;35m is sleeping     \033[0;0m\n"
# define	THINK	"\033[1;34m is thinking     \033[0;0m\n"
# define	EAT 	"\033[1;32m is eating       \033[0;0m\n"
# define	FORK	"\033[1;33m has taken a fork\033[0;0m\n"
# define	LEN		31

/*	
** remove color
*
# define	DEATH	"died            \n"
# define	SLEEP	"is sleeping     \n"
# define	THINK	"is thinking     \n"
# define	EAT 	"is eating       \n"
# define	FORK	"has taken a fork\n"
# define	LEN		17
*/

struct	s_the_table	
{
	struct timeval	time_start;
	pthread_t		*philos;

	uint32_t		time_die;
	uint32_t		time_eat;
	uint32_t		time_sleep;
	uint32_t		eat_limit;
	uint32_t		n_philo;
	char			limited_meals;
};

typedef struct s_philo
{
	struct s_the_table	*table;
	sem_t				*forks;
	sem_t				*display;
	int					pid;
	uint32_t			name;
	uint32_t			last_meal;
	uint32_t			meals_eated;
	char				is_dead;
}						t_philo;

/*
** lib
*/
uint32_t	ft_atoi(const char *s);
uint32_t	get_time(struct timeval *tv);

/*
**	Output
*/
void		annonce(const t_philo *philo, const char *message);

/*
**	init
*/
uint8_t		dress_table(struct s_the_table *table, t_philo **philosopher,
				char **av, const int ac);

/*
** Action
*/
void		take_forks(const t_philo *philo);
void		eat_(t_philo *philo);
void		sleep_(const t_philo *philo);
void		think_(const t_philo *philo);
void		*death_prediction(void *phi);
#endif
