/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 17:17:40 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/29 19:24:17 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_fork_odd(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	log_message(philo, "has taken a fork");
	if (philo->data->num_philosophers == 1)
	{
		ft_usleep(philo->data->time_to_die + 1);
		pthread_mutex_unlock(philo->left_fork);
		return ;
	}
	pthread_mutex_lock(philo->right_fork);
	log_message(philo, "has taken a fork");
}

void	philo_fork_even(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	log_message(philo, "has taken a fork");
	pthread_mutex_lock(philo->left_fork);
	log_message(philo, "has taken a fork");
}

void	philosopher_eat(t_philo *philo)
{
	log_message(philo, "is eating");
	pthread_mutex_lock(&philo->data->data_mutex);
	philo->last_meal_time = get_time_in_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->data_mutex);
	ft_usleep(philo->data->time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	start_check(t_philo *philo)
{
	while (1)
	{
		pthread_mutex_lock(&philo->data->data_mutex);
		if (philo->data->start_flag == 1)
		{
			pthread_mutex_unlock(&philo->data->data_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->data_mutex);
		usleep(50);
	}
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	start_check(philo);
	if (philo->id % 2 == 1 && philo->data->num_philosophers != 1)
		usleep(120);
	while (!flag_check(philo))
	{
		if (eat_check(philo))
			break ;
		if (philo->id % 2 == 1)
			philo_fork_odd(philo);
		else
			philo_fork_even(philo);
		philosopher_eat(philo);
		if (flag_check(philo))
			break ;
		log_message(philo, "is sleeping");
		ft_usleep(philo->data->time_to_sleep);
		log_message(philo, "is thinking");
		usleep(200);
	}
	return (NULL);
}
