/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yridgway <yridgway@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 22:23:04 by yridgway          #+#    #+#             */
/*   Updated: 2023/01/25 18:55:27 by yridgway         ###   ########.fr       */
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

t_philo	*init_philo(t_data *dat)
{
	t_philo		*philo;

	philo = malloc(sizeof (t_philo));
	if (!philo)
		return (write(2, "problem initializing philo\n", 28), NULL);
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
	return (philo);
}

void	*mythread(void *data)
{
	t_data		*dat;
	t_philo		*philo;

	dat = (t_data *)data;
	philo = init_philo(dat);
	if (!philo)
		return (NULL);
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
	i = -1;
	while (++i < data->num_philos)
		pthread_mutex_destroy(&data->fork_mutex[i]);
	pthread_mutex_destroy(&data->mutex);
	free(data->fork_mutex);
	free(data->philo_id);
	free(data->forks);
	free(data);
	return (0);
}
