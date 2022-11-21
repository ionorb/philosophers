/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yridgway <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 18:03:13 by yridgway          #+#    #+#             */
/*   Updated: 2022/11/07 19:55:21 by yridgway         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_death(t_data *dat, t_philo *philo)
{
//	pthread_mutex_lock(&dat->mutex);
	philo->is_dead = dat->death;
//	pthread_mutex_unlock(&dat->mutex);
	if (!philo->is_dead && ft_time(philo->begin_time)
		- philo->last_meal > philo->die_time)
	{
//		pthread_mutex_lock(&dat->mutex);
		printf("%ld %d died\n", ft_time(philo->begin_time), philo->id);
		dat->death = 1;
//		pthread_mutex_unlock(&dat->mutex);
	}
//	pthread_mutex_lock(&dat->mutex);
	philo->is_dead = dat->death;
//	pthread_mutex_unlock(&dat->mutex);
}

void	philo_eat(t_data *dat, t_philo *philo)
{
	pthread_mutex_lock(&dat->mutex);
	check_death(dat, philo);
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

void	ft_pickup(t_data *dat, t_philo *philo)
{
	int	fork1;
	int	fork2;

	fork1 = 1;
	fork2 = 1;
	while (!philo->is_dead && (fork1 || fork2))
	{
		pthread_mutex_lock(&dat->mutex);
		check_death(dat, philo);
		if (!philo->is_dead && dat->forks[philo->id - 1])
		{
			dat->forks[philo->id - 1] = 0;
			printf("%ld %d has taken a fork\n",
				ft_time(philo->begin_time), philo->id);
			fork1 = 0;
		}
		pthread_mutex_unlock(&dat->mutex);
		usleep(1);
		pthread_mutex_lock(&dat->mutex);
		check_death(dat, philo);
		if (!philo->is_dead && dat->forks[philo->id % dat->num_philos])
		{
			dat->forks[philo->id % dat->num_philos] = 0;
			printf("%ld %d has taken a fork\n",
				ft_time(philo->begin_time), philo->id);
			fork2 = 0;
		}
		pthread_mutex_unlock(&dat->mutex);
	}
}

void	philo_sleep(t_data *dat, t_philo *philo)
{	
	int	endtime;

	check_death(dat, philo);
	if (!philo->is_dead)
	{
		pthread_mutex_lock(&dat->mutex);
		check_death(dat, philo);
		if (!philo->is_dead)
			printf("%ld %d is sleeping\n", ft_time(philo->begin_time), philo->id);
		pthread_mutex_unlock(&dat->mutex);
	}
	endtime = ft_time(philo->begin_time) + philo->sleep_time;
	while (!philo->is_dead && ft_time(philo->begin_time) < endtime)
	{
		usleep(1000);
		pthread_mutex_lock(&dat->mutex);
		check_death(dat, philo);
		pthread_mutex_unlock(&dat->mutex);
	}
	if (!philo->is_dead)
	{
		pthread_mutex_lock(&dat->mutex);
		check_death(dat, philo);
		if (!philo->is_dead)
			printf("%ld %d is thinking\n", ft_time(philo->begin_time), philo->id);
		pthread_mutex_unlock(&dat->mutex);
	}
}

void	philo_does_things(t_data *dat, t_philo *philo)
{
	ft_pickup(dat, philo);
	pthread_mutex_lock(&dat->mutex);
	check_death(dat, philo);
	pthread_mutex_unlock(&dat->mutex);
	if (!philo->is_dead && philo->times_to_eat)
	{
		philo->last_meal = ft_time(philo->begin_time);
		philo_eat(dat, philo);
		philo->times_to_eat--;
		if (philo->times_to_eat != 0)
			philo_sleep(dat, philo);
	}
	pthread_mutex_lock(&dat->mutex);
	check_death(dat, philo);
	pthread_mutex_unlock(&dat->mutex);
}
