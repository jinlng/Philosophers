/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinliang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 13:00:58 by jinliang          #+#    #+#             */
/*   Updated: 2026/02/21 17:06:38 by jinliang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void *monitor_routine(void *arg)
{
    t_philo *philo;
    long long now;

    philo = (t_philo *)arg;

    while (1)
    {
        now = get_current_time();
		
        if (now - philo->last_meal >= philo->data->time_to_die)
        {
            sem_post(philo->data->death);  // 值=1，发送死亡信号
            print_status(philo, "died");
            exit(1);
        }
        usleep(500);
    }
    return (NULL);
}

// void	*monitor_routine(void *arg)
// {
// 	t_philo	*philo;
// 	long long	now;

// 	philo = (t_philo *)arg;

// 	while (1)
// 	{
// 		now = get_current_time();
// 		if (now - philo->last_meal >= philo->data->time_to_die)
// 		{
// 			// sem_wait(philo->data->death);
// 			sem_post(philo->data->death);
// 			print_status(philo, "died");
// 			exit(1);
// 		}
// 		usleep(1000);
// 	}
// 	return (NULL);
// }

void parent_monitor(t_data *data)
{
    int     status;
    int     finished;
    pid_t   pid;

    finished = 0;

    while ((pid = waitpid(-1, &status, 0)) > 0)
    {
        if (WIFEXITED(status))
        {
            if (WEXITSTATUS(status) == 0)
            {
                finished++;
                if (finished == data->num_philos)
                    clean_exit(data, 0);
            }
            else
            {
                clean_exit(data, 1);
            }
        }
    }
}




// void	*meal_monitor(void *arg)
// {
// 	t_philo	*philo;
// 	t_data	*data;
	
// 	philo = (t_philo *)arg;
// 	data = philo->data;
// 	if (data->must_eat_count <= 0)
// 		return (NULL);
// 	while (1)
// 	{
// 		usleep(1000);
// 		if (data->someone_died)
// 			break ;
// 		if (philo->eat_count >= data->must_eat_count)
// 			exit(0);
// 	}
// 	return (NULL);
// }

// void *death_monitor(void *arg)
// {
// 	t_philo		*philo;
// 	t_data		*data;
// 	long long	current_time;
// 	long long	time_since_last_meal;

// 	philo = (t_philo *)arg;
// 	data = (t_data *)philo->data;
// 	while (1)
// 	{
// 		usleep(1000);
// 		if (data->someone_died)
// 			break ;
// 		current_time = get_current_time();
// 		sem_wait(data->meal);
// 		time_since_last_meal = current_time - philo->last_meal;
// 		sem_post(data->meal);
// 		if (time_since_last_meal > data->time_to_die)
// 		{
// 			if (data->someone_died)
// 				exit(0);
// 			data->someone_died = 1;
// 			sem_wait(data->print);
// 			printf("%lld %d died\n",
// 				   current_time - data->start_time, philo->id);
// 			sem_post(data->print);
// 			sem_post(data->death);
// 			exit(1);
// 		}
// 	}
// 	return (NULL);
// }

