/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinliang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 19:25:16 by jinliang          #+#    #+#             */
/*   Updated: 2026/02/21 16:52:43 by jinliang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_data	*g_global_data = NULL;

t_data	*get_global_data(void)
{
	return (g_global_data);
}

void	handle_sigint(int sig)
{
    (void)sig;

    if (g_global_data)
    {
        // write(1, "\n[Signal] Cleaning up processes...\n", 35);
        kill_all(g_global_data);
        // destroy_semaphores(g_global_data);
		clean_exit(g_global_data, 1);
        exit(0);
    }
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
		return (printf("Error: Usage: ./philo number_of_philosophers \
			time_to_die time_to_eat time_to_sleep [must_eat_count]"), 1);
	if (!init_data(&data, argc, argv))
		return (printf("Error: Initialization failed"), 1);
	g_global_data = &data;
	signal(SIGINT, handle_sigint);
	// if (data.num_philos == 1)
	// 		return (solo_philo(&data), 0);
	data.start_time = get_current_time();
	start_simulation(&data);
	clean_exit(&data, 0);
	return (0);
}


void start_simulation(t_data *data)
{
    create_processes(data);    // 🔹 先 fork 所有 philosopher
    parent_monitor(data);      // 🔹 父进程监控子进程

    // 所有 philosopher 正常结束或有死亡被捕获
    clean_exit(data, 0);       // 🔹 正常退出
}

