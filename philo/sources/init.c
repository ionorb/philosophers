/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yridgway <yridgway@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 18:37:50 by yridgway          #+#    #+#             */
/*   Updated: 2023/01/28 20:27:44 by yridgway         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	ft_exit_msg(char *msg)
{
	printf("%s\n", msg);
	exit(1);
}

int	init_id(t_data *data, char **av)
{
	int	i;

	i = -1;
	data->num_philos = ft_atoi(av[1]);
	data->philo_id = malloc(data->num_philos * sizeof (pthread_t));
	if (!data->philo_id)
		return (write(2, "problem initializing philo_id\n", 31), -1);
	return (0);
}

int	init_forks(t_data *data)
{
	int	i;

	i = -1;
	data->forks = malloc(data->num_philos * sizeof (int));
	if (!data->forks)
		return (free(data->philo_id),
			write(2, "problem initializing fork array\n", 33), -1);
	while (++i < data->num_philos)
		data->forks[i] = 1;
	return (0);
}

int	init_mutex(t_data *data)
{
	int	i;

	i = 0;
	if (pthread_mutex_init(&data->mutex, NULL))
		return (free(data->forks), free(data->philo_id), \
		write(2, "problem initializing mutex\n", 28), -1);
	data->fork_mutex = malloc(sizeof (pthread_mutex_t) * data->num_philos);
	if (!data->fork_mutex)
		return (pthread_mutex_destroy(&data->mutex), free(data->forks), \
		free(data->philo_id), \
		write(2, "problem initializing fork_mutex\n", 33), -1);
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->fork_mutex[i], NULL))
		{
			while (i >= 0)
				pthread_mutex_destroy(&data->fork_mutex[i--]);
			pthread_mutex_destroy(&data->mutex);
			return (free(data->fork_mutex), free(data->forks), \
			free(data->philo_id), \
			write(2, "problem initializing a fork mutex\n", 33), -1);
		}
		i++;
	}
	return (0);
}

int	init_data(int ac, char **av, t_data *data)
{
	struct timeval	current_time;

	if (init_id(data, av) || init_forks(data) || init_mutex(data))
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
