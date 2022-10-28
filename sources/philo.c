/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yridgway <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 18:03:13 by yridgway          #+#    #+#             */
/*   Updated: 2022/10/28 23:26:49 by yridgway         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*mythread(t_data *data)
{
	int	i;

	i = 0;
//	write(2, "ban\n", 4);
	while (i++ < 1000)
	{
		pthread_mutex_lock(&data->mutex);
		data->counter += 1;
		pthread_mutex_unlock(&data->mutex);
	}
	sleep(1);
	return NULL;
}

void	ft_exit_msg(char *msg)
{
	ft_putstr_fd(msg, 2);
	exit(1);
}

t_data	*init_data(int ac, char **av)
{
	t_data	*data;

	if (ac < 5)
		ft_exit_msg("not enough arguments");
	data = malloc(sizeof (data));
	data->number_of_philosophers = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->counter = 0;
	pthread_mutex_init(&data->mutex, NULL);
	return (data);
}

int	main(int ac, char **av) 
{
	pthread_t	*philo_id;
	t_data		*data;
	int			i;

	data = init_data(ac, av);
	if (!data)
		ft_exit_msg("problem initializing data");
	philo_id = malloc(data->number_of_philosophers * sizeof (pthread_t));
	i = -1;
	while (++i < data->number_of_philosophers)
		pthread_create(&philo_id[i], NULL, mythread, data);
	i = -1;
	while (++i < data->number_of_philosophers)
		pthread_join(philo_id[i], NULL);
	pthread_mutex_destroy(&data->mutex);
	printf("after %d\n", data->counter);
	exit(0);
}
