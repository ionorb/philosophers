/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yridgway <yridgway@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 18:03:13 by yridgway          #+#    #+#             */
/*   Updated: 2023/01/26 22:23:56 by yridgway         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	check_death(t_data *dat, t_philo *philo)
{
	if (!dat->death && (ft_time(philo->begin_time) - \
	philo->last_meal > philo->die_time || philo->die_time == 0))
	{
		printf("%ld %d died\n", ft_time(philo->begin_time), philo->id);
		dat->death = 1;
	}
	philo->is_dead = dat->death;
}

void	philo_eat(t_data *dat, t_philo *philo)
{
	int	endtime;

	write_philo_msg(philo, dat, "is eating");
	endtime = ft_time(philo->begin_time) + philo->eat_time;
	while (!philo->is_dead && ft_time(philo->begin_time) < endtime)
	{
		usleep(1000);
		pthread_mutex_lock(&dat->mutex);
		check_death(dat, philo);
		pthread_mutex_unlock(&dat->mutex);
	}
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
		write_philo_msg(philo, dat, "has taken a fork");
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
		pthread_mutex_lock(&dat->mutex);
		check_death(dat, philo);
		pthread_mutex_unlock(&dat->mutex);
		pthread_mutex_lock(&dat->fork_mutex[philo->id % philo->num_philos]);
		take_fork(dat, philo, 2);
		pthread_mutex_unlock(&dat->fork_mutex[philo->id % philo->num_philos]);
	}
}

void	philo_sleep(t_data *dat, t_philo *philo)
{	
	int	endtime;

	write_philo_msg(philo, dat, "is sleeping");
	endtime = ft_time(philo->begin_time) + philo->sleep_time;
	while (!philo->is_dead && ft_time(philo->begin_time) < endtime)
	{
		usleep(1000);
		pthread_mutex_lock(&dat->mutex);
		check_death(dat, philo);
		pthread_mutex_unlock(&dat->mutex);
	}
	write_philo_msg(philo, dat, "is thinking");
}
