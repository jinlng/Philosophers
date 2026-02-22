/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinliang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 14:06:33 by jinliang          #+#    #+#             */
/*   Updated: 2026/02/19 16:09:40 by jinliang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	init_data(t_data *data, int argc, char **argv)
{
	data->num_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->start_time = get_current_time();
	data->must_eat_count = -1;
	if (argc == 6)
		data->must_eat_count = ft_atoi(argv[5]);
	if (data->num_philos <= 0 || data->time_to_die <= 0
		|| data->time_to_eat <= 0 || data->time_to_sleep <= 0
		|| (argc == 6 && data->must_eat_count <= 0))
		return (0);
	data->pids = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->pids)
		return (0);
	data->is_parent = 1;
	if (!init_semaphores(data))
		return (0);
	return (1);
}

int	init_semaphores(t_data *data)
{
	sem_unlink("/philo_forks");
	sem_unlink("/philo_print"); 
	sem_unlink("/philo_death");
	// sem_unlink("/philo_meal");
	data->forks = sem_open("/philo_forks", O_CREAT, 0644, data->num_philos);
	data->print = sem_open("/philo_print", O_CREAT, 0644, 1);
	data->death = sem_open("/philo_death", O_CREAT, 0644, 0);
	// data->meal = sem_open("/philo_meal", O_CREAT, 0644, 1);
	if (data->forks == SEM_FAILED
		|| data->print == SEM_FAILED
		|| data->death == SEM_FAILED)
		return (printf("Error: Semaphore initialization failed\n"), 0);
	return (1);
}
