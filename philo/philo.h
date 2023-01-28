/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yridgway <yridgway@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:51:45 by yridgway          #+#    #+#             */
/*   Updated: 2023/01/28 20:27:04 by yridgway         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>

// extern	int	global;

typedef struct s_data
{
	int				num_philos;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				counter;
	int				*forks;
	int				death;
	int				times_to_eat;
	long int		begin_time;
	pthread_t		*philo_id;
	pthread_mutex_t	mutex;
	pthread_mutex_t	*fork_mutex;
}	t_data;

typedef struct s_philo
{
	long int	begin_time;
	int			last_meal;
	int			longest_wait;
	int			id;
	int			is_dead;
	int			die_time;
	int			eat_time;
	int			sleep_time;
	int			time_since_meal;
	int			times_to_eat;
	int			fork1;
	int			fork2;
	int			num_philos;
}	t_philo;

//utils
void		write_philo_msg(t_philo *philo, t_data *dat, char *msg);
long int	ft_time(long int begin_time);
void		ft_putstr_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);
long		ft_atoi(const char *nptr);
void		ft_free(t_data *data);

//init
int			init_data(int ac, char **av, t_data *data);
void		ft_exit_msg(char *msg);

//actions
void		check_death(t_data *dat, t_philo *philo);
void		philo_eat(t_data *dat, t_philo *philo);
void		philo_sleep(t_data *dat, t_philo *philo);
void		ft_pickup(t_data *dat, t_philo *philo);

// void	*ft_malloc_fail(long size);

#endif
