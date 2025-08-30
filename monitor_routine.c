/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 17:16:42 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/29 19:23:29 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_max_eaten(t_data *data)
{
	int	i;
	int	all_eaten_enough;

	if (data->max_meal == -1)
		return (0);
	pthread_mutex_lock(&data->data_mutex);
	all_eaten_enough = 0;
	i = 0;
	while (i < data->num_philosophers)
	{
		if (data->philosophers[i].meals_eaten >= data->max_meal)
			all_eaten_enough++;
		i++;
	}
	if (all_eaten_enough == data->num_philosophers)
	{
		data->similation_end = 1;
		pthread_mutex_unlock(&data->data_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->data_mutex);
	return (0);
}

int	check_death(t_data *data, int i)
{
	long	current_time;
	long	hungry_time;

	pthread_mutex_lock(&data->data_mutex);
	current_time = get_time_in_ms();
	hungry_time = current_time - data->philosophers[i].last_meal_time;
	if (hungry_time >= data->time_to_die)
	{
		data->similation_end = 1;
		printf("%ld %d %s\n", current_time - data->similatuion_start_time,
			data->philosophers[i].id, "died");
		pthread_mutex_unlock(&data->data_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->data_mutex);
	return (0);
}

void	*monitor_routine(void *args)
{
	t_data	*data;
	int		i;

	data = (t_data *)args;
	i = 0;
	while (1)
	{
		i = 0;
		if (check_max_eaten(data))
			return (NULL);
		while (i < data->num_philosophers)
		{
			if (check_death(data, i))
				return (NULL);
			i++;
		}
	}
	return (NULL);
}
