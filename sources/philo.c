/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yridgway <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 18:03:13 by yridgway          #+#    #+#             */
/*   Updated: 2022/10/31 22:00:33 by yridgway         ###   ########.fr       */
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
	int	is_dead;

	is_dead = 0;
	ft_pickup(dat, philo);
	philo_eat(dat, philo);
	philo->last_meal = ft_time(philo->begin_time) - philo->begin_time;
//	printf("lastmeal %d\n", *last_meal);
	ft_putdown(dat, philo);
	philo_sleep(dat, philo);
	return (is_dead);
}

void	*mythread(void *data)
{
	t_data		*dat;
	t_philo		*philo;
	int			is_dead;

	dat = data;
	philo = malloc(sizeof (t_philo));
	pthread_mutex_lock(&dat->mutex);
	dat->counter++;
	philo->id = dat->counter;
	philo->begin_time = dat->begin_time;
	pthread_mutex_unlock(&dat->mutex);
	is_dead = 0;
	philo->last_meal = ft_time(philo->begin_time);
	if (!(philo->id % 2))
		usleep(100);
	while (!is_dead)
		is_dead = philo_does_things(dat, philo);
//	sleep(0.9);
	return (NULL);
}

int	main(int ac, char **av)
{
	pthread_t	*philo_id;
	t_data		*data;
	int			i;

	data = init_data(ac, av);
	if (!data)
		ft_exit_msg("problem initializing data");
	philo_id = malloc(((t_data *)data)->num_philos * sizeof (pthread_t));
	i = -1;
	while (++i < data->num_philos)
		pthread_create(&philo_id[i], NULL, mythread, (void *)data);
	i = -1;
	while (++i < data->num_philos)
		pthread_join(philo_id[i], NULL);
	pthread_mutex_destroy(&data->mutex);
	printf("fork[0]: %d, counter: %d\n", data->forks[0], data->counter);
	return (0);
}
