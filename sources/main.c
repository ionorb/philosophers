/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yridgway <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 22:23:04 by yridgway          #+#    #+#             */
/*   Updated: 2022/11/22 16:02:45 by yridgway         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	philo->num_philos = dat->num_philos;
	pthread_mutex_unlock(&dat->mutex);
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
	while (++i < data->num_philos)
		pthread_create(&data->philo_id[i], NULL, mythread, (void *)data);
	i = -1;
	while (++i < data->num_philos)
		pthread_join(data->philo_id[i], NULL);
	pthread_mutex_destroy(&data->mutex);
	free(data->philo_id);
	free(data->forks);
	free(data);
	return (0);
}
