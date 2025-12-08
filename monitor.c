/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinliang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 10:52:48 by jinliang          #+#    #+#             */
/*   Updated: 2025/12/07 10:55:48 by jinliang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	start_simulation(t_data *data)
{
	int	i;

	for (i = 0; i < data->num_philos; i++)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
						philo_routine, &data->philos[i]) != 0)
		{
			print_error("Error: Failed to create thread");
			return;
		}
	}

	monitor_loop(data);

	for (i = 0; i < data->num_philos; i++)
	{
		if (pthread_join(data->philos[i].thread, NULL) != 0)
			print_error("Error: Failed to join thread");
	}
}

void	monitor_loop(t_data *data)
{
	int	i;
	int	all_ate_enough;

	while (1)
	{
		if (check_philosopher_death(data))
			break;

		if (data->must_eat_count > 0)
		{
			all_ate_enough = 1;
			pthread_mutex_lock(&data->death_mutex);
			for (i = 0; i < data->num_philos; i++)
			{
				if (data->philos[i].meal_count < data->must_eat_count)
				{
					all_ate_enough = 0;
					break;
				}
			}
			pthread_mutex_unlock(&data->death_mutex);

			if (all_ate_enough)
				break;
		}
		usleep(1000);
	}
}

int	check_philosopher_death(t_data *data)
{
	int			i;
	long long	current_time;

	for (i = 0; i < data->num_philos; i++)
	{
		pthread_mutex_lock(&data->death_mutex);
		current_time = get_current_time();

		if (current_time - data->philos[i].last_meal_time > data->time_to_die)
		{
			data->someone_died = 1;
			pthread_mutex_unlock(&data->death_mutex);

			pthread_mutex_lock(&data->print_mutex);
			printf("%lld %d died\\n",
				   current_time - data->start_time, data->philos[i].id);
			pthread_mutex_unlock(&data->print_mutex);

			return (1);
		}
		pthread_mutex_unlock(&data->death_mutex);
	}
	return (0);
}

void	cleanup(t_data *data)
{
	int	i;

	if (data->forks)
	{
		for (i = 0; i < data->num_philos; i++)
			pthread_mutex_destroy(&data->forks[i]);
		free(data->forks);
	}

	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_mutex);

	if (data->philos)
		free(data->philos);
}
