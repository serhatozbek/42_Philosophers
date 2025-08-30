/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 17:17:52 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/29 19:28:27 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_usleep(long duration_ms)
{
	long	start_time;

	start_time = get_time_in_ms();
	while ((get_time_in_ms() - start_time) < duration_ms)
	{
		usleep(100);
	}
}

void	destroy_all(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->data_mutex);
	free(data->forks);
	free(data->philosophers);
}

void	log_message(t_philo *philo, const char *msg)
{
	if (flag_check(philo))
		return ;
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%ld %d %s\n", get_time_in_ms()
		- philo->data->similatuion_start_time, philo->id, msg);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

void	join_threads(t_data *data, pthread_t *monitor_thread)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers)
	{
		pthread_join(data->philosophers[i].thread, NULL);
		i++;
	}
	pthread_join(*monitor_thread, NULL);
	destroy_all(data);
}
