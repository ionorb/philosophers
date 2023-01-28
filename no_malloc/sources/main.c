/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yridgway <yridgway@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 22:23:04 by yridgway          #+#    #+#             */
/*   Updated: 2023/01/28 22:30:23 by yridgway         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	philo_does_things(t_data *dat, t_philo *philo)
{
	pthread_mutex_lock(&dat->mutex);
	check_death(dat, philo);
	pthread_mutex_unlock(&dat->mutex);
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

void	init_philo(t_data *dat, t_philo *philo)
{
	pthread_mutex_lock(&dat->mutex);
	dat->counter++;
	philo->id = dat->counter;
	philo->begin_time = dat->begin_time;
	philo->die_time = dat->die_time;
	philo->eat_time = dat->eat_time;
	philo->sleep_time = dat->sleep_time;
	philo->times_to_eat = dat->times_to_eat;
	philo->num_philos = dat->num_philos;
	pthread_mutex_unlock(&dat->mutex);
	philo->is_dead = 0;
	philo->last_meal = ft_time(philo->begin_time);
	philo->longest_wait = 0;
}

void	*mythread(void *data)
{
	t_data		*dat;
	t_philo		philo;

	dat = (t_data *)data;
	init_philo(dat, &philo);
	if (!(philo.id % 2))
		usleep(10000);
	while (!philo.is_dead && philo.times_to_eat)
		philo_does_things(dat, &philo);
	return (NULL);
}

int	init_data(int ac, char **av, t_data *data)
{
	struct timeval	current_time;
	int				i;

	i = -1;
	data->num_philos = ft_atoi(av[1]);
	while (++i < data->num_philos)
		data->forks[i] = 1;
	if (init_mutex(data))
		return (1);
	data->die_time = ft_atoi(av[2]);
	data->eat_time = ft_atoi(av[3]);
	data->sleep_time = ft_atoi(av[4]);
	data->times_to_eat = -1;
	if (ac == 6)
		data->times_to_eat = ft_atoi(av[5]);
	data->counter = 0;
	data->death = 0;
	gettimeofday(&current_time, NULL);
	data->begin_time = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	return (0);
}

int	main(int ac, char **av)
{
	t_data			data;
	int				i;

	if (ft_check_args(ac, av))
		return (1);
	if (init_data(ac, av, &data))
		return (1);
	i = -1;
	while (++i < data.num_philos)
	{
		if (pthread_create(&data.philo_id[i], NULL, mythread, (void *)&data))
			return (write(1, "problem creating threads\n", 26), 1);
	}
	i = -1;
	while (++i < data.num_philos)
	{
		if (pthread_join(data.philo_id[i], NULL))
			return (write(1, "problem joining threads\n", 25), 1);
	}
	return (0);
}
