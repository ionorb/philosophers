/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yridgway <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 18:03:13 by yridgway          #+#    #+#             */
/*   Updated: 2022/10/31 21:12:29 by yridgway         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long int	*ft_time(void)
{
	long int	*timi;
	struct timeval current_time;

	timi = malloc(sizeof (int) * 2);
	gettimeofday(&current_time, NULL);
	timi[1] = current_time.tv_sec;
	timi[2] = current_time.tv_usec;
	return (timi);
}

void	philo_eat(t_data *dat, int id)
{
	printf("<%ld.%ld> %d is eating\n", ft_time()[1], ft_time()[2], id);
	usleep(dat->eat_time * 1000);
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
		if (dat->forks[id - 1])
		{
		//	printf("fork[%d]: %d\n", id, dat->forks[id]);
			dat->forks[id - 1] = 0;
			printf("<%ld.%ld> %d has taken fork %d\n", ft_time()[1], ft_time()[2], id, id - 1);
			fork1 = 0;
		}
		if (dat->forks[id % dat->num_philos])
		{
		//	printf("fork[%d]: %d\n", (id - 1) % dat->num_philos, dat->forks[(id - 1) % dat->num_philos]);
			dat->forks[id  % dat->num_philos] = 0;
			printf("<%ld.%ld> %d has taken fork %d\n", ft_time()[1], ft_time()[2], id, id % dat->num_philos);
			fork2 = 0;
		}
		pthread_mutex_unlock(&dat->mutex);
	}
}

void	ft_putdown(t_data *dat, int id)
{
	pthread_mutex_lock(&dat->mutex);
	dat->forks[id - 1] = 1;
	pthread_mutex_unlock(&dat->mutex);
	printf("<%ld.%ld> %d has put down fork %d\n", ft_time()[1], ft_time()[2], id, id - 1);
	pthread_mutex_lock(&dat->mutex);
	dat->forks[id % dat->num_philos] = 1;
	pthread_mutex_unlock(&dat->mutex);
	printf("<%ld.%ld> %d has put down fork %d\n", ft_time()[1], ft_time()[2], id, id % dat->num_philos);
}

void	philo_sleep(t_data *dat, int id)
{
	printf("<%ld.%ld> %d is sleeping\n", ft_time()[1], ft_time()[2], id);
	usleep(dat->sleep_time * 1000);
	printf("<%ld.%ld> %d is thinking\n", ft_time()[1], ft_time()[2], id);
}

int	philo_does_things(t_data *dat, int id)
{
	int	is_dead;

	is_dead = 0;
	ft_pickup(dat, id);
	philo_eat(dat, id);
	ft_putdown(dat, id);
	philo_sleep(dat, id);
	return (is_dead);
}

void	*mythread(void *data)
{
	t_data	*dat;
	int		is_dead;
	int		id;

	dat = data;
	pthread_mutex_lock(&dat->mutex);
	dat->counter++;
	id = dat->counter;
	pthread_mutex_unlock(&dat->mutex);
	is_dead = 0;
	if (!(id % 2))
		usleep(100);
	while (!is_dead)
		is_dead = philo_does_things(dat, id);
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
