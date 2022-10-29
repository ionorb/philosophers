/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yridgway <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 18:03:13 by yridgway          #+#    #+#             */
/*   Updated: 2022/10/29 19:59:00 by yridgway         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_eat(t_data *dat, int id)
{
	int	fork_right;
	int	fork_left;

	if (id == 1)
	{
		fork_right = dat->num_philos - 1;
		fork_left = 0;
	}
	else
	{
		fork_right = id - 2;
		fork_left = id - 1;
	}
	if (forks[fork_right] == 1 || forks[fork_left] == 1)
		write(2, "oh no\n", 6);
	else
	{
		printf("%d has taken a fork\n", id);
		dat->forks[fork_right] = 1;
		printf("%d has taken a fork\n", id);
		dat->forks[fork_left] = 1;
	}
	printf("%d is eating\n", id);
	sleep(dat->eat_time / 1000);
}

void	philo_offset(t_data *dat, int id)
{
	if ((id % 2))
		sleep(1);
	if (id == 1)
		sleep(1);
}

void	philo_sleep(t_data *dat, int id)
{
}

void	philo_does_things(t_data *dat, int id)
{
	philo_eat(dat, id);
	philo_sleep(dat);
}

void	*mythread(void *data)
{
	t_data	*dat;
	int		is_dead;
	int		id;
	int		i;

	i = -1;
	is_alive = 1;
	dat = data;
	pthread_mutex_lock(&dat->mutex);
	dat->counter++;
	id = dat->counter;
	pthread_mutex_unlock(&dat->mutex);
	pthread_mutex_lock(&dat->mutex);
	if (dat->death)
		return (NULL);
	pthread_mutex_unlock(&dat->mutex);
	is_dead = 0;
	philo_offset(dat, id);
	while (!is_dead)
	{
		pthread_mutex_lock(&dat->mutex);
		philo_does_things(dat, id);
		pthread_mutex_unlock(&dat->mutex);
	}
	pthread_mutex_lock(&dat->mutex);
	dat->death = is_dead;
	pthread_mutex_unlock(&dat->mutex);
//	sleep(0.9);
	return (NULL);
}

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
	t_data	*data;

	if (ac < 5)
		ft_exit_msg("not enough arguments");
	data = malloc(sizeof (t_data));
	data->num_philos = ft_atoi(av[1]);
	data->die_time = ft_atoi(av[2]);
	data->eat_time = ft_atoi(av[3]);
	data->sleep_time = ft_atoi(av[4]);
	data->counter = 0;
	data->death = 0;
//	data->cycle_length = data->sleep_time + data->eat_time
	pthread_mutex_init(&data->mutex, NULL);
	init_forks(data);
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
