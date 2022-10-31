/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yridgway <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 18:03:13 by yridgway          #+#    #+#             */
/*   Updated: 2022/10/31 18:34:06 by yridgway         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_time(void)
{
	struct timeval current_time;
	gettimeofday(&current_time, NULL);
	return (current_time.tv_sec - 1667236200);
}

void	philo_eat(t_data *dat, int id)
{
//	int	fork_right;
//	int	fork_left;

//	fork_right = (i - 1) % dat->num_philos;
//	fork_left = i;
//	if (forks[fork_right] == 1 || forks[fork_left] == 1)
//		write(2, "oh no\n", 6);
//		dat->forks[fork_right] = 1;
//		dat->forks[fork_left] = 1;
	(void)id;
	printf("<%d> %d is eating\n", ft_time(), id);
	sleep(dat->eat_time / 1000);
}

void	ft_pickup(t_data *dat, int id)
{
	int	fork1;
	int	fork2;

	fork1 = 1;
	fork2 = 1;
	while (fork1 || fork2)
	{
		pthread_mutex_lock(&dat->mutex);
		if (dat->forks[id])
		{
			dat->forks[id] = 0;
			printf("<%d> %d has taken fork %d\n", ft_time(), id, id);
			fork1 = 0;
		}
		if (dat->forks[(id - 1) % dat->num_philos])
		{
			dat->forks[(id - 1) % dat->num_philos] = 0;
			printf("<%d> %d has taken fork %d\n", ft_time(), id, (id - 1) % dat->num_philos);
			fork2 = 0;
		}
		pthread_mutex_unlock(&dat->mutex);
	}
}

void	ft_putdown(t_data *dat, int id)
{
	pthread_mutex_lock(&dat->mutex);
	dat->forks[id] = 1;
	pthread_mutex_unlock(&dat->mutex);
	printf("<%d> %d has put down a fork\n", ft_time(), id);
	pthread_mutex_lock(&dat->mutex);
	dat->forks[(id - 1) % dat->num_philos] = 1;
	pthread_mutex_unlock(&dat->mutex);
	printf("<%d> %d has put down a fork\n", ft_time(), id);
}

void	philo_sleep(t_data *dat, int id)
{
	printf("<%d> %d is sleeping\n", ft_time(), id);
	sleep(dat->sleep_time / 1000);
	printf("<%d> %d is thinking\n", ft_time(), id);
}

void	wait_for_others(t_data *dat)
{
	while (1)
	{
		pthread_mutex_lock(&dat->mutex);
		if (dat->sitting == dat->num_philos - 1)
			break ;
		pthread_mutex_unlock(&dat->mutex);
	//	sleep(1);
	}
}

void	philo_does_things(t_data *dat, int id, int *is_sitting)
{
	pthread_mutex_lock(&dat->mutex);
	if (*is_sitting && dat->sitting == dat->num_philos)
	{
		*is_sitting = 0;
		dat->sitting -= 1;
	//	printf("<%d> %d stood up\n", ft_time(), id);
	}
	else if (!*is_sitting)
	{
		*is_sitting = 1;
		dat->sitting += 1;
	//	printf("<%d> %d sat down\n", ft_time(), id);
	}
	if (*is_sitting && dat->sitting == dat->num_philos - 1)
	{
		pthread_mutex_unlock(&dat->mutex);
		ft_pickup(dat, id);
		philo_eat(dat, id);
		ft_putdown(dat, id);
		philo_sleep(dat, id);
		pthread_mutex_lock(&dat->mutex);
	}
//	printf("sitting: %d\n", dat->sitting);
	pthread_mutex_unlock(&dat->mutex);
}

void	*mythread(void *data)
{
	t_data	*dat;
	int		is_dead;
	int		is_sitting;
	int		id;

	is_dead = 0;
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
	is_sitting = 1;
//	philo_offset(dat, id);
	while (!is_dead)
	{
		philo_does_things(dat, id, &is_sitting);
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
	data->sitting = data->num_philos;
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
