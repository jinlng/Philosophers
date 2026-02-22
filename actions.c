/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinliang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 10:52:14 by jinliang          #+#    #+#             */
/*   Updated: 2026/02/22 15:16:08 by jinliang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
	}
}

void	philo_eat(t_philo *philo)
{
	take_forks(philo);
	pthread_mutex_lock(&philo->data->death_mutex);
	philo->last_meal_time = get_current_time();
	philo->meal_count++;
	pthread_mutex_unlock(&philo->data->death_mutex);
	print_status(philo, "is eating");
	smart_sleep(philo->data->time_to_eat, philo->data);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	usleep(200);
}

void	philo_sleep(t_philo *philo)
{
	print_status(philo, "is sleeping");
	smart_sleep(philo->data->time_to_sleep, philo->data);
}

void	philo_think(t_philo *philo)
{
	long long	think_time;
	long long	time_since_meal;
	long long	safe_margin;

	print_status(philo, "is thinking");
	time_since_meal = get_current_time() - philo->last_meal_time;
	safe_margin = philo->data->time_to_die - time_since_meal
		- philo->data->time_to_eat;
	think_time = safe_margin / 3;
	if (think_time < 0)
		think_time = 0;
	if (think_time > philo->data->time_to_eat)
	{
		if (think_time > 300)
			think_time = 100;
		else if (think_time > 200)
			think_time = 50;
		else if (think_time > 100)
			think_time = 20;
		else
			think_time = 0;
	}
	smart_sleep(think_time, philo->data);
}
