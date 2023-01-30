/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yridgway <yridgway@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 22:37:38 by yridgway          #+#    #+#             */
/*   Updated: 2023/01/29 15:00:51 by yridgway         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	ft_check_args(int ac, char **av)
{
	int	i;
	int	j;

	if (ac < 5 || ac > 6)
		return (printf("wrong number of arguments\n"), 1);
	if (ft_atoi(av[1]) < 1)
		return (printf("number of philosophers must be at least 1\n"), 1);
	if (ft_atoi(av[1]) > 200)
		return (printf("number of philos must be less than 201\n"), 1);
	while (--ac)
	{
		i = -1;
		j = 0;
		while (av[ac][++i])
		{
			if (av[ac][i] < '0' || av[ac][i] > '9')
				return (printf("arguments must contain only digits\n"), 1);
			j++;
		}
		if (j > 10)
			return (printf("arguments must be less than 10 digits\n"), 1);
		if (ft_atoi(av[ac]) > INT_MAX)
			return (printf("arguments must be less than INT_MAX\n"), 1);
	}
	return (0);
}

long int	ft_time(long int begin_time)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return (current_time.tv_sec * 1000
		+ current_time.tv_usec / 1000 - begin_time);
}

void	write_philo_msg(t_philo *philo, t_data *dat, char *msg)
{
	pthread_mutex_lock(&dat->mutex);
	check_death(dat, philo);
	if (!philo->is_dead)
		printf("%ld %d %s\n", ft_time(philo->begin_time), philo->id, msg);
	pthread_mutex_unlock(&dat->mutex);
}

long	ft_atoi(const char *nptr)
{
	long	i;
	long	sign;
	long	result;

	result = 0;
	sign = 1;
	i = 0;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '+')
		i++;
	else if (nptr[i] == '-')
	{
		sign = -1;
		i++;
	}
	while (nptr[i] >= 48 && nptr[i] <= 57)
	{
		result *= 10;
		result += (nptr[i] - '0');
		i++;
	}
	return (result * sign);
}

int	init_mutex(t_data *data)
{
	int	i;

	i = 0;
	if (pthread_mutex_init(&data->mutex, NULL))
		return (write(2, "problem initializing mutex\n", 28), -1);
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->fork_mutex[i], NULL))
		{
			while (--i >= 0)
				pthread_mutex_destroy(&data->fork_mutex[i]);
			pthread_mutex_destroy(&data->mutex);
			return (write(2, "problem initializing fork mutex\n", 33), -1);
		}
		i++;
	}
	return (0);
}
