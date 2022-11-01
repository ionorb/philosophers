/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yridgway <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 18:37:50 by yridgway          #+#    #+#             */
/*   Updated: 2022/11/01 15:00:44 by yridgway         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_exit_msg(char *msg)
{
	ft_putstr_fd(msg, 2);
	exit(1);
}

void	init_forks(t_data *data)
{
	int	i;

	i = -1;
	data->forks = malloc(data->num_philos * sizeof (int));
	while (++i < data->num_philos)
		data->forks[i] = 1;
}

t_data	*init_data(int ac, char **av)
{
	t_data			*data;
	struct timeval	current_time;

	if (ac < 5)
		ft_exit_msg("not enough arguments");
	data = malloc(sizeof (t_data));
	if (pthread_mutex_init(&data->mutex, NULL) != 0)
	{
		printf("\n\nohno\n\n");
	}
	data->num_philos = ft_atoi(av[1]);
	data->die_time = ft_atoi(av[2]);
	data->eat_time = ft_atoi(av[3]);
	data->sleep_time = ft_atoi(av[4]);
	data->counter = 0;
	init_forks(data);
	data->death = 0;
	gettimeofday(&current_time, NULL);
	data->begin_time = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	return (data);
}
