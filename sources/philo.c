/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yridgway <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 18:03:13 by yridgway          #+#    #+#             */
/*   Updated: 2022/10/31 23:07:41 by yridgway         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_eat(t_data *dat, t_philo *philo)
{
	printf("<%ld> %d is eating\n", ft_time(philo->begin_time), philo->id);
	usleep(dat->eat_time * 1000);
}

void	ft_pickup(t_data *dat, t_philo *philo)
{
	int	fork1;
	int	fork2;

	fork1 = 1;
	fork2 = 1;
	while (fork1 || fork2)
	{
		pthread_mutex_lock(&dat->mutex);
		if (dat->forks[philo->id - 1])
		{
		//	printf("fork[%d]: %d\n", id, dat->forks[id]);
			dat->forks[philo->id - 1] = 0;
			printf("<%ld> %d has taken fork %d\n", ft_time(philo->begin_time), philo->id, philo->id - 1);
			fork1 = 0;
		}
		if (dat->forks[philo->id % dat->num_philos])
		{
		//	printf("fork[%d]: %d\n", (id - 1) % dat->num_philos, dat->forks[(id - 1) % dat->num_philos]);
			dat->forks[philo->id  % dat->num_philos] = 0;
			printf("<%ld> %d has taken fork %d\n", ft_time(philo->begin_time), philo->id, philo->id % dat->num_philos);
			fork2 = 0;
		}
		pthread_mutex_unlock(&dat->mutex);
	}
}

void	ft_putdown(t_data *dat, t_philo *philo)
{
	pthread_mutex_lock(&dat->mutex);
	dat->forks[philo->id - 1] = 1;
	printf("<%ld> %d has put down fork %d\n", ft_time(philo->begin_time), philo->id, philo->id - 1);
	pthread_mutex_unlock(&dat->mutex);
	pthread_mutex_lock(&dat->mutex);
	dat->forks[philo->id % dat->num_philos] = 1;
	printf("<%ld> %d has put down fork %d\n", ft_time(philo->begin_time), philo->id, philo->id % dat->num_philos);
	pthread_mutex_unlock(&dat->mutex);
}

void	philo_sleep(t_data *dat, t_philo *philo)
{
	printf("<%ld> %d is sleeping\n", ft_time(philo->begin_time), philo->id);
	usleep(dat->sleep_time * 1000);
	printf("<%ld> %d is thinking\n", ft_time(philo->begin_time), philo->id);
}

int	philo_does_things(t_data *dat, t_philo *philo)
{
	ft_pickup(dat, philo);
	printf("time since lastmeal %ld\n", ft_time(philo->begin_time) - philo->last_meal);
	if (ft_time(philo->begin_time) - philo->last_meal > philo->longest_wait)
		philo->longest_wait = ft_time(philo->begin_time) - philo->last_meal;
	printf("longest wait: %d\n", philo->longest_wait);
	philo_eat(dat, philo);
	philo->last_meal = ft_time(philo->begin_time);
	ft_putdown(dat, philo);
	philo_sleep(dat, philo);
	pthread_mutex_lock(&dat->mutex);
	if (ft_time(philo->begin_time) - philo->last_meal > dat->die_time)
		dat->death = 1;
	philo->is_dead = dat->death;
	pthread_mutex_unlock(&dat->mutex);
	return (philo->is_dead);
}
