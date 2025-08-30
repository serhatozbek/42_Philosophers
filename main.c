/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 14:59:31 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/29 19:22:44 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **args)
{
	t_data		data;
	pthread_t	monitor_thread;
	int			i;

	i = 0;
	if (parse_args(ac, args, &data))
		return (1);
	if (all_init(&data))
		return (1);
	if (pthread_create(&monitor_thread, NULL, monitor_routine, &data) != 0)
	{
		pthread_mutex_lock(&data.data_mutex);
		data.similation_end = 1;
		pthread_mutex_unlock(&data.data_mutex);
		i = 0;
		while (i < data.num_philosophers)
		{
			pthread_join(data.philosophers[i].thread, NULL);
			i++;
		}
		destroy_all(&data);
		return (1);
	}
	join_threads(&data, &monitor_thread);
	return (0);
}
