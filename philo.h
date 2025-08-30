/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 14:49:46 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/29 19:34:22 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_data	*data;
}					t_philo;

typedef struct s_data
{
	int				num_philosophers;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				start_flag;
	int				max_meal;
	long			similatuion_start_time;
	int				similation_end;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	data_mutex;
	t_philo			*philosophers;
}					t_data;

int					init_data(t_data *data);
int					init_mutexs(t_data *data);
int					init_philosophers(t_data *data);
int					parse_args(int ac, char **args, t_data *data);
int					all_init(t_data *data);
int					start_philos(t_data *data);
int					start_timer(t_data *data);

void				*philosopher_routine(void *arg);
void				*monitor_routine(void *args);
int					flag_check(t_philo *philo);
int					eat_check(t_philo *philo);
int					ft_atoi(const char *str);

long				get_time_in_ms(void);
void				ft_usleep(long duration_ms);
void				destroy_all(t_data *data);
void				log_message(t_philo *philo, const char *msg);
int					flag_check(t_philo *philo);
void				join_threads(t_data *data, pthread_t *monitor_thread);

#endif
