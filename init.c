/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinliang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 10:46:39 by jinliang          #+#    #+#             */
/*   Updated: 2025/12/07 10:54:29 by jinliang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_data(t_data *data, int argc, char **argv)
{
	data->num_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);

	if (argc == 6)
		data->must_eat_count = ft_atoi(argv[5]);
	else
		data->must_eat_count = -1;

	if (data->num_philos <= 0 || data->time_to_die <= 0 ||
		data->time_to_eat <= 0 || data->time_to_sleep <= 0 ||
		(argc == 6 && data->must_eat_count <= 0))
		return (0);

	if (!init_mutexes(data))
		return (0);

	if (!init_philosophers(data))
		return (0);

	data->start_time = get_current_time();
	data->someone_died = 0;
	return (1);
}

int	init_mutexes(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
		return (0);

	for (i = 0; i < data->num_philos; i++)
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (0);

	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (0);

	if (pthread_mutex_init(&data->death_mutex, NULL) != 0)
		return (0);

	return (1);
}

int	init_philosophers(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
		return (0);

	for (i = 0; i < data->num_philos; i++)
	{
		data->philos[i].id = i + 1;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
		data->philos[i].last_meal_time = get_current_time();
		data->philos[i].meal_count = 0;
		data->philos[i].data = data;
	}

	return (1);
}
