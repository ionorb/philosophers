/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yridgway <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:51:45 by yridgway          #+#    #+#             */
/*   Updated: 2022/10/31 21:59:36 by yridgway         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_data
{
	int				num_philos;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				cycle_length;
	int				counter;
	int				*forks;
	int				sitting;
	int				death;
	long int		begin_time;
	pthread_mutex_t	mutex;
}	t_data;

typedef struct s_philo
{
	long int	begin_time;
	int			last_meal;
	int			id;
}	t_philo;

//utils
long int	ft_time(long int begin_time);
void	ft_putstr_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
int		ft_atoi(const char *nptr);

//init
t_data	*init_data(int ac, char **av);
void	init_forks(t_data *data);
void	ft_exit_msg(char *msg);
#endif
