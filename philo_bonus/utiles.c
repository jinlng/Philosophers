/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utiles.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinliang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 12:51:16 by jinliang          #+#    #+#             */
/*   Updated: 2026/02/21 16:53:32 by jinliang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void kill_all(t_data *data)
{
    int i;
    
    // printf("DEBUG: Killing all philosopher processes\n");
    
    // 1. 先发 SIGTERM 让进程有机会清理
    for (i = 0; i < data->num_philos; i++)
    {
        if (data->pids[i] > 0)
        {
            kill(data->pids[i], SIGTERM);
            // printf("DEBUG: Sent SIGTERM to philosopher %d (pid: %d)\n", 
            //        i + 1, data->pids[i]);
        }
    }
    
    // 2. 给一点时间退出
    usleep(10000);  // 10ms
    
    // 3. 强制杀死还在运行的
    for (i = 0; i < data->num_philos; i++)
    {
        if (data->pids[i] > 0)
        {
            // 检查进程是否还在
            if (kill(data->pids[i], 0) == 0)
            {
                kill(data->pids[i], SIGKILL);
                // printf("DEBUG: Sent SIGKILL to philosopher %d (pid: %d)\n", 
                //        i + 1, data->pids[i]);
            }
        }
    }
    
    // 4. 等待所有子进程结束，避免僵尸进程
    for (i = 0; i < data->num_philos; i++)
    {
        if (data->pids[i] > 0)
        {
            waitpid(data->pids[i], NULL, 0);
            // printf("DEBUG: Philosopher %d process reaped\n", i + 1);
        }
    }
    
    // printf("DEBUG: All philosopher processes killed\n");
}

static void cleanup_semaphores(t_data *data)
{
    if (data->forks && data->forks != SEM_FAILED)
    {
        sem_close(data->forks);
        data->forks = NULL;
    }
    if (data->print && data->print != SEM_FAILED)
    {
        sem_close(data->print);
        data->print = NULL;
    }
    if (data->death && data->death != SEM_FAILED)
    {
        sem_close(data->death);
        data->death = NULL;
    }
    
    sem_unlink("/philo_forks");
    sem_unlink("/philo_print");
    sem_unlink("/philo_death");
}

void clean_exit(t_data *data, int exit_code)
{
    static int cleaning = 0;
    
    if (cleaning)
        return;
    cleaning = 1;
    
    // printf("DEBUG: Cleaning up with exit code %d...\n", exit_code);

    if (exit_code != 0)
    {
        kill_all(data);
        cleanup_semaphores(data);
        // printf("DEBUG: Cleanup complete, exiting with code %d\n", exit_code);
        exit(exit_code);
    }
    cleanup_semaphores(data);

    if (data->pids)
        free(data->pids);
    if (data->philos)
        free(data->philos);
    // printf("DEBUG: Normal cleanup complete, exiting with code %d\n", exit_code);
    cleaning = 0;
	exit(exit_code);
}

// void	clean_exit(t_data *data, int exit_code)
// {
// 	static int cleaning = 0;
    
//     if (cleaning)
//         return;
//     cleaning = 1;
// 	if (!data)
// 		exit(exit_code);
// 	printf("DEBUG: Cleaning up with exit code %d...\n", exit_code);
// 	// 只有 parent 负责真正清理世界
// 	if (data->is_parent)
// 	{
// 		// 如果异常退出，杀掉所有 child
// 		if (exit_code != 0)
// 			kill_all(data);

// 		// 回收子进程（防 zombie）
// 		while (waitpid(-1, NULL, 0) > 0)
//         	;

// 		// 关闭 semaphore
// 		if (data->forks && data->forks != SEM_FAILED)
//             sem_close(data->forks);
//         if (data->print && data->print != SEM_FAILED)
//             sem_close(data->print);
//         if (data->death && data->death != SEM_FAILED)
//             sem_close(data->death);

// 		// 删除 semaphore
// 		sem_unlink("/philo_forks");
// 		sem_unlink("/philo_print");
// 		sem_unlink("/philo_death");

// 		// free 内存
// 		if (data->pids)
// 			free(data->pids);
// 	}
// 	else
// 	{
// 		// child 只关闭，不 unlink
// 		if (data->forks != SEM_FAILED)
// 			sem_close(data->forks);
// 		if (data->print != SEM_FAILED)
// 			sem_close(data->print);
// 		if (data->death != SEM_FAILED)
// 			sem_close(data->death);
// 	}
// 	printf("DEBUG: Cleanup complete, exiting with code %d\n", exit_code);
//     cleaning = 0;
// 	exit(exit_code);
// }

// void	clean_exit(t_data *data, int exit_code)
// {
// 	kill_all(data);
// 	destroy_semaphores(data);
// 	if (data->philos)
// 	{
// 		free(data->philos);
// 		data->philos = NULL;
// 	}
// 	exit(exit_code);
// }


// void clean_exit(t_data *data, int exit_code)
// {
//     static int cleaning = 0;
    
//     if (cleaning)
//         return;
//     cleaning = 1;
    
//     // printf("DEBUG: Cleaning up with exit code %d...\n", exit_code);
    
//     // ✅ 只有异常退出（exit_code != 0）才需要强制杀死进程
//     if (exit_code != 0)
//     {
//         kill_all(data);
//     }
//     else
//     {
// 		exit(0);
//         // printf("DEBUG: Normal exit, no need to kill\n");
//     }
    
//     // 关闭信号量
//     if (data->forks != SEM_FAILED)
//         sem_close(data->forks);
//     if (data->print != SEM_FAILED)
//         sem_close(data->print);
//     if (data->death != SEM_FAILED)
//         sem_close(data->death);
    
//     // 删除信号量
//     sem_unlink("/philo_forks");
//     sem_unlink("/philo_print");
//     sem_unlink("/philo_death");
//     free(data->pids);
//     // printf("DEBUG: Cleanup complete\n");
//     cleaning = 0;
//     exit(exit_code);
// }

// void	kill_all(t_data *data)
// {
// 	int i;

// 	// printf("DEBUG: Killing all philosopher processes\n");
// 	i = -1;
// 	while (++i < data->num_philos)
// 	{
// 		if (data->philos[i].pid > 0)
// 		{
// 			kill(data->philos[i].pid, SIGKILL);
// 			// printf("DEBUG: Sent SIGKILL to philosopher %d (pid: %d)\n", 
//             //        i + 1, data->philos[i].pid);
// 		}
// 	}
// 	i = -1;
// 	while (++i < data->num_philos)
// 	{
// 		if (data->philos[i].pid > 0)
// 		{
// 			waitpid(data->philos[i].pid, NULL, 0);
//             // printf("DEBUG: Philosopher %d process reaped\n", i + 1);
// 		}
// 	}
// }
// void kill_all(t_data *data)
// {
//     int i;
//     int killed = 0;
    
//     printf("DEBUG: Killing remaining philosopher processes\n");
    
//     for (i = 0; i < data->num_philos; i++)
//     {
//         if (data->philos[i].pid > 0)
//         {
//             // ✅ 检查进程是否还在运行
//             if (kill(data->philos[i].pid, 0) == 0)
//             {
//                 kill(data->philos[i].pid, SIGKILL);
//                 waitpid(data->philos[i].pid, NULL, 0);
//                 killed++;
//                 printf("DEBUG: Killed philosopher %d (pid: %d)\n", 
//                        i + 1, data->philos[i].pid);
//             }
//             else
//             {
//                 printf("DEBUG: Philosopher %d already exited\n", i + 1);
//             }
//         }
//     }
    
//     printf("DEBUG: Killed %d remaining philosophers\n", killed);
// }

// void destroy_semaphores(t_data *data)
// {
//     if (data->forks)
//     {
//         sem_close(data->forks);
//         sem_unlink("/philo_forks");
//     }
//     if (data->print)
//     {
//         sem_close(data->print);
//         sem_unlink("/philo_print");
//     }
//     if (data->death)
//     {
//         sem_close(data->death);
//         sem_unlink("/philo_death");
//     }
//     if (data->meal)
//     {
//         sem_close(data->meal);
//         sem_unlink("/philo_meal");
//     }
// }

int	ft_atoi(const char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (sign * result);
}

long long	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void print_status(t_philo *philo, char *status)
{
    t_data		*data;
    long long	timestamp;
    
	data = (t_data *)philo->data;
    timestamp = get_current_time() - data->start_time;
    sem_wait(data->print);
    printf("%lld %d %s\n", timestamp, philo->id, status);
    sem_post(data->print);
}
