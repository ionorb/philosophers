/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yridgway <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 18:03:13 by yridgway          #+#    #+#             */
/*   Updated: 2022/10/31 18:43:55 by yridgway         ###   ########.fr       */
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

int	philo_does_things(t_data *dat, int id, int *is_sitting)
{
	int	is_dead;

	is_dead = 0;
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
	return (is_dead);
}

void	*mythread(void *data)
{
	t_data	*dat;
	int		is_dead;
	int		is_sitting;
	int		id;

	dat = data;
	pthread_mutex_lock(&dat->mutex);
	dat->counter++;
	id = dat->counter;
	pthread_mutex_unlock(&dat->mutex);
	is_dead = 0;
	is_sitting = 1;
	while (!is_dead)
		is_dead = philo_does_things(dat, id, &is_sitting);
//	sleep(0.9);
	return (NULL);
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
