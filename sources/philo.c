/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yridgway <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 18:03:13 by yridgway          #+#    #+#             */
/*   Updated: 2022/10/31 17:09:46 by yridgway         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_eat(t_data *dat, int id)
{
//	int	fork_right;
//	int	fork_left;

//	fork_right = (i - 1) % dat->num_philos;
//	fork_left = i;
//	if (forks[fork_right] == 1 || forks[fork_left] == 1)
//		write(2, "oh no\n", 6);
	if (1)
	{
		printf("%d has taken a fork\n", id);
//		dat->forks[fork_right] = 1;
		printf("%d has taken a fork\n", id);
//		dat->forks[fork_left] = 1;
	}
	printf("%d is eating\n", id);
	sleep(dat->eat_time / 1000);
	printf("%d has put down a fork\n", id);
	printf("%d has put down a fork\n", id);
}

/*void	philo_offset(t_data *dat, int id)
{
	if ((id % 2))
		sleep(1);
	if (id == 1)
		sleep(1);
}*/

void	philo_sleep(t_data *dat, int id)
{
	sleep(dat->sleep_time / 1000);
	printf("%d is eating\n", id);
}

void	philo_does_things(t_data *dat, int id, int *is_sitting)
{
	pthread_mutex_lock(&dat->mutex);
	if (*is_sitting && dat->sitting == dat->num_philos - 1)
	{
		pthread_mutex_unlock(&dat->mutex);
		//ft_pickup();
		philo_eat(dat, id);
		philo_sleep(dat, id);
		//ft_putdown();
		pthread_mutex_lock(&dat->mutex);
	}
	else if (*is_sitting && dat->sitting == dat->num_philos)
	{
		*is_sitting = 0;
		dat->sitting -= 1;
	//	printf("philo %d stood up\n", id);
	}
	else
	{
		*is_sitting = 1;
		dat->sitting += 1;
		wait_for_others()
	//	printf("philo %d sat down\n", id);
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
