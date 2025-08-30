/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.42kocaeli.tr>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-08-29 16:17:35 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/29 19:19:13 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_data(t_data *data)
{
	data->similatuion_start_time = get_time_in_ms();
	data->similation_end = 0;
	data->start_flag = 0;
	data->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* data->num_philosophers);
	if (!data->forks)
		return (1);
	data->philosophers = (t_philo *)malloc(sizeof(t_philo)
			* data->num_philosophers);
	if (!data->philosophers)
		return (free(data->forks), 1);
	return (0);
}

int	init_mutexs(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			while (i > 0)
			{
				i--;
				pthread_mutex_destroy(&data->forks[i]);
			}
			return (1);
		}
		i++;
	}
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&data->data_mutex, NULL) != 0)
		return (1);
	return (0);
}

int	init_philosophers(t_data *data)
{
	int	i;

	i = 0;
	i = 0;
	while (i < data->num_philosophers)
	{
		data->philosophers[i].id = i + 1;
		data->philosophers[i].meals_eaten = 0;
		data->philosophers[i].last_meal_time = data->similatuion_start_time;
		data->philosophers[i].data = data;
		data->philosophers[i].left_fork = &data->forks[i];
		data->philosophers[i].right_fork = &data->forks[(i + 1)
			% data->num_philosophers];
		i++;
	}
	return (0);
}

int	parse_args(int ac, char **args, t_data *data)
{
	if (ac < 5 || ac > 6)
	{
		printf("Arguman Error\n");
		return (1);
	}
	data->num_philosophers = ft_atoi(args[1]);
	data->similation_end = 0;
	data->time_to_die = ft_atoi(args[2]);
	data->time_to_eat = ft_atoi(args[3]);
	data->time_to_sleep = ft_atoi(args[4]);
	data->max_meal = -1;
	if (ac == 6)
		data->max_meal = ft_atoi(args[5]);
	if (data->num_philosophers <= 0 || data->time_to_die < 0
		|| data->time_to_eat < 0 || data->time_to_sleep < 0 || (ac == 6
			&& data->max_meal < 0))
	{
		printf("Arguman Error\n");
		return (1);
	}
	return (0);
}

int	all_init(t_data *data)
{
	if (init_data(data))
		return (1);
	if (init_mutexs(data) != 0)
	{
		printf("Hata Mutexler başlatılamadı.\n");
		destroy_all(data);
		return (1);
	}
	if (init_philosophers(data) != 0)
	{
		printf("Hata: Felsefeciler başlatılamadı.\n");
		destroy_all(data);
		return (1);
	}
	if (start_philos(data))
		return (1);
	return (0);
}
