/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinliang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 10:52:48 by jinliang          #+#    #+#             */
/*   Updated: 2026/02/22 15:17:38 by jinliang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	start_simulation(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL, \
			philo_routine, &data->philos[i]) != 0)
		{
			printf("Error: Failed to create thread");
			return ;
		}
	}
	monitor_loop(data);
	i = -1;
	while (++i < data->num_philos)
	{
		if (pthread_join(data->philos[i].thread, NULL) != 0)
			printf("Error: Failed to join thread");
	}
}

void	monitor_loop(t_data *data)
{
	int	i;

	while (1)
	{
		if (check_death(data))
			break ;
		if (data->must_eat_count > 0)
		{
			pthread_mutex_lock(&data->death_mutex);
			i = -1;
			while (++i < data->num_philos)
			{
				if (data->philos[i].meal_count < data->must_eat_count)
					break ;
			}
			pthread_mutex_unlock(&data->death_mutex);
			if (i == data->num_philos)
				break ;
		}
		usleep(500);
	}
}

int	check_death(t_data *data)
{
	int			i;

	i = -1;
	while (++i < data->num_philos)
	{
		pthread_mutex_lock(&data->death_mutex);
		if (!(data->must_eat_count > 0
				&& data->philos[i].meal_count >= data->must_eat_count)
			&& get_current_time() - data->philos[i].last_meal_time
			> data->time_to_die)
		{
			if (!data->someone_died)
			{
				data->someone_died = 1;
				pthread_mutex_lock(&data->print_mutex);
				printf("%lld %d died\n", get_current_time() - data->start_time,
					data->philos[i].id);
				pthread_mutex_unlock(&data->print_mutex);
			}
			pthread_mutex_unlock(&data->death_mutex);
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
		i = -1;
		while (++i < data->num_philos)
			pthread_mutex_destroy(&data->forks[i]);
		free(data->forks);
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_mutex);
	if (data->philos)
		free(data->philos);
}
