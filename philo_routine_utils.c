/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.42kocaeli.tr>       #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-08-29 16:24:56 by sozbek            #+#    #+#             */
/*   Updated: 2025-08-29 16:24:56 by sozbek           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(int c)
{
	return (c <= '9' && c >= '0');
}

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	res;

	i = 0;
	sign = 1;
	res = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i])
	{
		if (!(ft_isdigit(str[i])))
			return (res * sign);
		else
			res = res * 10 + str[i] - 48;
		i++;
	}
	return (res * sign);
}

int	flag_check(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->data_mutex);
	if (philo->data->similation_end != 0)
	{
		pthread_mutex_unlock(&philo->data->data_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->data_mutex);
	return (0);
}

int	eat_check(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->data_mutex);
	if (philo->data->max_meal != -1
		&& philo->meals_eaten >= philo->data->max_meal)
	{
		pthread_mutex_unlock(&philo->data->data_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->data_mutex);
	return (0);
}
