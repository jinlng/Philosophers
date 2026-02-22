/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinliang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 15:34:35 by jinliang          #+#    #+#             */
/*   Updated: 2026/02/21 17:15:37 by jinliang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

// void	create_all_philosophers(t_data *data)
// {
// 	int	i;

// 	i = 0;
// 	while (i < data->num_philos)
// 	{
// 		create_process(data, i);  // fork新进程
// 		i++;
// 		usleep(100);
// 	}
// 	// printf("DEBUG: All philosopher processes created\n");
// }

// // 创建单个哲学家进程
// void	create_process(t_data *data, int i)
// {
// 	data->philos[i].pid = fork();

// 	 if (data->philos[i].pid < 0)
//     {
//         perror("fork");
//         exit(1);
//     }
	
// 	if (data->philos[i].pid == 0)
// 	{
// 		// printf("DEBUG: Philosopher %d process REALLY started (pid: %d)\n", 
// 		// 		   data->philos[i].id, getpid());
// 		fflush(stdout);
// 		// 进入主循环
// 		philo_routine(&data->philos[i]);  // 吃饭睡觉思考
// 		exit(0);
// 	}
// 	else
// 	{
// 		// printf("DEBUG: Created philosopher %d with PID: %d\n", 
//         //            i + 1, data->philos[i].pid);
// 		fflush(stdout);
// 	}
// }

void	create_processes(t_data *data)
{
	int	i;
	pid_t	pid;

	data->start_time = get_current_time();
	i = -1;
	while (++i < data->num_philos)
	{
		pid = fork();
		if (pid == 0)
		{
			data->is_parent = 0;
			child_process(data, i);
			// printf("DEBUG: child %d pid=%d started\n", i + 1, getpid());
			exit(0);
		}
		else if (pid > 0)
			data->pids[i] = pid;
		else
			exit(1);
	}
	usleep(100);
}

void	child_process(t_data *data, int index)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
    if (!philo)
		exit(1);
	philo->id = index + 1;
    philo->eat_count = 0;
    philo->last_meal = get_current_time();
    philo->data = data;

    data->is_parent = 0;

    pthread_mutex_init(&philo->meal_lock, NULL);

	if (data->num_philos == 1)
    {
        sem_wait(data->forks);
        print_status(philo, "has taken a fork");
        smart_sleep(data->time_to_die, philo);
        print_status(philo, "died");
        exit(1);
    }

    pthread_create(&philo->monitor, NULL, monitor_routine, philo);
    pthread_detach(philo->monitor);

    philo_routine(philo);

    pthread_mutex_destroy(&philo->meal_lock);
    // free(philo);
    exit(0);
}

// void	kill_all(t_data *data)
// {
// 	int	i;

// 	if (!data->pids)
// 		return ;

// 	i = 0;
// 	while (i < data->num_philos)
// 	{
// 		if (data->pids[i] > 0)
// 			kill(data->pids[i], SIGKILL);
// 		i++;
// 	}
// }
