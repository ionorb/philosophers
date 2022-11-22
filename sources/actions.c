/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yridgway <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 18:03:13 by yridgway          #+#    #+#             */
/*   Updated: 2022/11/22 16:02:51 by yridgway         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_death(t_data *dat, t_philo *philo)
{
	if (!dat->death && ft_time(philo->begin_time)
		- philo->last_meal > philo->die_time)
	{
		printf("%ld %d died\n", ft_time(philo->begin_time), philo->id);
		dat->death = 1;
	}
	philo->is_dead = dat->death;
}

void	philo_eat(t_data *dat, t_philo *philo)
{
	pthread_mutex_lock(&dat->mutex);
	check_death(dat, philo);
	if (!philo->is_dead)
		printf("%ld %d is eating\n", ft_time(philo->begin_time), philo->id);
	pthread_mutex_unlock(&dat->mutex);
	usleep(philo->eat_time * 1000);
	pthread_mutex_lock(&dat->fork_mutex[philo->id - 1]);
	dat->forks[philo->id - 1] = 1;
	pthread_mutex_unlock(&dat->fork_mutex[philo->id - 1]);
	pthread_mutex_lock(&dat->fork_mutex[philo->id % philo->num_philos]);
	dat->forks[philo->id % philo->num_philos] = 1;
	pthread_mutex_unlock(&dat->fork_mutex[philo->id % philo->num_philos]);
}

void	take_fork(t_data *dat, t_philo *philo, int i)
{
	int	index;

	if (i == 1)
		index = philo->id - 1;
	else
		index = philo->id % philo->num_philos;
	if (!philo->is_dead && dat->forks[index])
	{
		dat->forks[index] = 0;
		pthread_mutex_lock(&dat->mutex);
		check_death(dat, philo);
		if (!philo->is_dead)
			printf("%ld %d has taken a fork\n",
				ft_time(philo->begin_time), philo->id);
		pthread_mutex_unlock(&dat->mutex);
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
		pthread_mutex_lock(&dat->fork_mutex[philo->id - 1]);
		take_fork(dat, philo, 1);
		pthread_mutex_unlock(&dat->fork_mutex[philo->id - 1]);
		usleep(1);
		pthread_mutex_lock(&dat->fork_mutex[philo->id % philo->num_philos]);
		take_fork(dat, philo, 2);
		pthread_mutex_unlock(&dat->fork_mutex[philo->id % philo->num_philos]);
	}
}

void	philo_sleep(t_data *dat, t_philo *philo)
{	
	int	endtime;

	pthread_mutex_lock(&dat->mutex);
	check_death(dat, philo);
	if (!philo->is_dead)
		printf("%ld %d is sleeping\n",
			ft_time(philo->begin_time), philo->id);
	pthread_mutex_unlock(&dat->mutex);
	endtime = ft_time(philo->begin_time) + philo->sleep_time;
	while (!philo->is_dead && ft_time(philo->begin_time) < endtime)
	{
		usleep(1000);
		pthread_mutex_lock(&dat->mutex);
		check_death(dat, philo);
		pthread_mutex_unlock(&dat->mutex);
	}
	pthread_mutex_lock(&dat->mutex);
	check_death(dat, philo);
	if (!philo->is_dead)
		printf("%ld %d is thinking\n",
			ft_time(philo->begin_time), philo->id);
	pthread_mutex_unlock(&dat->mutex);
}
