/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yridgway <yridgway@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 22:37:38 by yridgway          #+#    #+#             */
/*   Updated: 2023/01/28 20:27:08 by yridgway         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

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

int	ft_whitespace(char c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

long	ft_atoi(const char *nptr)
{
	long	i;
	long	sign;
	long	result;

	result = 0;
	sign = 1;
	i = 0;
	while (ft_whitespace(nptr[i]))
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

void	ft_free(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
		pthread_mutex_destroy(&data->fork_mutex[i++]);
	pthread_mutex_destroy(&data->mutex);
	free(data->fork_mutex);
	free(data->philo_id);
	free(data->forks);
}

// void	*ft_malloc_fail(long size)
// {
// 	static int	i = 0;

// 	i++;
// 	if (global--)
// 		return (malloc(size));
// 	return (printf("failed on malloc no. %d\n", i), NULL);
// }
