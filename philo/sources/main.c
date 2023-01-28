/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yridgway <yridgway@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 22:23:04 by yridgway          #+#    #+#             */
/*   Updated: 2023/01/28 20:26:55 by yridgway         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

// int	global = 3;

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

void	ft_check_args(int ac, char **av)
{
	int	i;
	int	j;

	if (ac < 5)
		ft_exit_msg("not enough arguments");
	if (ac > 6)
		ft_exit_msg("too many arguments");
	if (ft_atoi(av[1]) < 1)
		ft_exit_msg("number of philosophers must be at least 1");
	while (--ac)
	{
		i = -1;
		j = 0;
		while (av[ac][++i])
		{
			if (av[ac][i] < '0' || av[ac][i] > '9')
				ft_exit_msg("arguments must contain only digits");
			j++;
		}
		if (j > 10)
			ft_exit_msg("arguments must be less than 10 digits");
		if (ft_atoi(av[ac]) > INT_MAX)
			ft_exit_msg("arguments must be less than INT_MAX (2,147,483,647)");
	}
}

int	main(int ac, char **av)
{
	t_data			data;
	int				i;

	ft_check_args(ac, av);
	if (init_data(ac, av, &data))
		return (1);
	i = -1;
	while (++i < data.num_philos)
	{
		if (pthread_create(&data.philo_id[i], NULL, mythread, (void *)&data))
			return (ft_free(&data), \
			write(1, "problem creating threads\n", 26), 1);
	}
	i = -1;
	while (++i < data.num_philos)
	{
		if (pthread_join(data.philo_id[i], NULL))
			return (ft_free(&data), \
			write(1, "problem joining threads\n", 25), 1);
	}
	return (ft_free(&data), 0);
}
