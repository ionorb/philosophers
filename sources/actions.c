/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yridgway <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 18:03:13 by yridgway          #+#    #+#             */
/*   Updated: 2022/11/21 21:54:58 by yridgway         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_death(t_data *dat, t_philo *philo)
{
	pthread_mutex_lock(&dat->mutex);
	philo->is_dead = dat->death;
	if (!philo->is_dead && ft_time(philo->begin_time)
		- philo->last_meal > philo->die_time)
	{
		printf("%ld %d died\n", ft_time(philo->begin_time), philo->id);
		dat->death = 1;
	}
	philo->is_dead = dat->death;
	pthread_mutex_unlock(&dat->mutex);
}

void	philo_eat(t_data *dat, t_philo *philo)
{
	check_death(dat, philo);
	pthread_mutex_lock(&dat->mutex);
	if (!philo->is_dead)
		printf("%ld %d is eating\n", ft_time(philo->begin_time), philo->id);
	pthread_mutex_unlock(&dat->mutex);
	usleep(philo->eat_time * 1000);
	pthread_mutex_lock(&dat->mutex);
	dat->forks[philo->id - 1] = 1;
	pthread_mutex_unlock(&dat->mutex);
	pthread_mutex_lock(&dat->mutex);
	dat->forks[philo->id % dat->num_philos] = 1;
	pthread_mutex_unlock(&dat->mutex);
}

void	take_fork(t_data *dat, t_philo *philo, int i)
{
	int	index;

	if (i == 1)
		index = philo->id - 1;
	else
		index = philo->id % dat->num_philos;
	if (!philo->is_dead && dat->forks[index])
	{
		dat->forks[index] = 0;
		printf("%ld %d has taken a fork\n",
			ft_time(philo->begin_time), philo->id);
		if (i == 1)
			philo->fork1 = 0;
		else
			philo->fork2 = 0;
	}
}

void	ft_pickup(t_data *dat, t_philo *philo)
{
	philo->fork1 = 1;
	philo->fork2 = 1;
	while (!philo->is_dead && (philo->fork1 || philo->fork2))
	{
		check_death(dat, philo);
		pthread_mutex_lock(&dat->mutex);
		take_fork(dat, philo, 1);
		pthread_mutex_unlock(&dat->mutex);
		usleep(1);
		check_death(dat, philo);
		pthread_mutex_lock(&dat->mutex);
		take_fork(dat, philo, 2);
		pthread_mutex_unlock(&dat->mutex);
	}
}

void	philo_sleep(t_data *dat, t_philo *philo)
{	
	int	endtime;

	check_death(dat, philo);
	pthread_mutex_lock(&dat->mutex);
	if (!philo->is_dead)
		printf("%ld %d is sleeping\n",
			ft_time(philo->begin_time), philo->id);
	pthread_mutex_unlock(&dat->mutex);
	endtime = ft_time(philo->begin_time) + philo->sleep_time;
	while (!philo->is_dead && ft_time(philo->begin_time) < endtime)
	{
		usleep(1000);
		check_death(dat, philo);
	}
	check_death(dat, philo);
	pthread_mutex_lock(&dat->mutex);
	if (!philo->is_dead)
		printf("%ld %d is thinking\n",
			ft_time(philo->begin_time), philo->id);
	pthread_mutex_unlock(&dat->mutex);
}
