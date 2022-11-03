/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yridgway <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 22:23:04 by yridgway          #+#    #+#             */
/*   Updated: 2022/11/03 18:33:24 by yridgway         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*mythread(void *data)
{
	t_data		*dat;
	t_philo		*philo;

	dat = data;
	philo = malloc(sizeof (t_philo));
	pthread_mutex_lock(&dat->mutex);
	dat->counter++;
	philo->id = dat->counter;
	philo->begin_time = dat->begin_time;
	philo->die_time = dat->die_time;
	philo->eat_time = dat->eat_time;
	philo->sleep_time = dat->sleep_time;
	philo->times_to_eat = dat->times_to_eat;
	pthread_mutex_unlock(&dat->mutex);
	pthread_mutex_lock(&dat->starting_stall);
	while (!dat->start)
	{
	}
	pthread_mutex_unlock(&dat->starting_stall);
	philo->is_dead = 0;
	philo->last_meal = ft_time(philo->begin_time);
	philo->longest_wait = 0;
	if (!(philo->id % 2))
		usleep(10000);
	while (!philo->is_dead && philo->times_to_eat)
		philo_does_things(dat, philo);
	free(philo);
	return (NULL);
}

int	main(int ac, char **av)
{
	t_data			*data;
	int				i;


	data = init_data(ac, av);
	if (!data)
		ft_exit_msg("problem initializing data");
	i = -1;
	pthread_mutex_init(&data->starting_stall, NULL);
	data->start = 0;
	while (++i < data->num_philos)
		pthread_create(&data->philo_id[i], NULL, mythread, (void *)data);
	data->start = 1;
	i = -1;
	while (++i < data->num_philos)
		pthread_join(data->philo_id[i], NULL);
	pthread_mutex_destroy(&data->mutex);
	pthread_mutex_destroy(&data->starting_stall);
	free(data->philo_id);
	free(data->forks);
	free(data);
	return (0);
}
