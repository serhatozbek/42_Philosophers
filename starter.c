/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   starter.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 22:44:20 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/29 19:27:09 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	start_philos(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	while (i < data->num_philosophers)
	{
		if (pthread_create(&data->philosophers[i].thread, NULL,
				philosopher_routine, &data->philosophers[i]) != 0)
		{
			pthread_mutex_lock(&data->data_mutex);
			data->similation_end = 1;
			pthread_mutex_unlock(&data->data_mutex);
			j = 0;
			while (j < i)
			{
				pthread_join(data->philosophers[j].thread, NULL);
				j++;
			}
			destroy_all(data);
			return (1);
		}
		i++;
	}
	start_timer(data);
	return (0);
}

int	start_timer(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&data->data_mutex);
	data->similatuion_start_time = get_time_in_ms();
	while (i < data->num_philosophers)
	{
		data->philosophers[i].last_meal_time = data->similatuion_start_time;
		i++;
	}
	data->start_flag = 1;
	pthread_mutex_unlock(&data->data_mutex);
	return (0);
}
