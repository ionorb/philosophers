/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yridgway <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 18:37:50 by yridgway          #+#    #+#             */
/*   Updated: 2022/11/03 19:00:48 by yridgway         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_exit_msg(char *msg)
{
	ft_putstr_fd(msg, 2);
	exit(1);
}

int	init_forks(t_data *data, char **av)
{
	int	i;

	i = -1;
	data->num_philos = ft_atoi(av[1]);
	data->forks = malloc(data->num_philos * sizeof (int));
	if (!data->forks)
		return (-1);
	while (++i < data->num_philos)
		data->forks[i] = 1;
	return (0);
}

int	init_mutex(t_data *data)
{
	if (pthread_mutex_init(&data->mutex, NULL) != 0)
	{
		ft_putstr_fd("", 2);
		return (-1);
	}
	return (0);
}

int	init_id(t_data *data)
{
	data->philo_id = malloc(data->num_philos * sizeof (pthread_t));
	if (!data->philo_id)
		return (-1);
	return (0);
}

t_data	*init_data(int ac, char **av)
{
	t_data			*data;
	struct timeval	current_time;

	if (ac < 5)
		ft_exit_msg("not enough arguments");
	data = malloc(sizeof (t_data));
	if (!data || init_mutex(data) || init_forks(data, av) || init_id(data))
		return (NULL);
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
	return (data);
}
