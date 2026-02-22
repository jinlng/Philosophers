/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinliang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 19:25:55 by jinliang          #+#    #+#             */
/*   Updated: 2026/02/22 15:24:46 by jinliang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void philo_routine(t_philo *philo)
{
    if (philo->id % 2 == 0)
        usleep(1000);
    while (1)
    {
		if (check_death_signal(philo))
            exit(0);
        // {
        //  if (sem_trywait(philo->data->death) == 0)
        //    // 收到了死亡信号！有人死了
        //     sem_post(philo->data->death);  // 让其他人也能看到
        //     exit(0);  // 正常退出
        // }
        print_status(philo, "is thinking");
        philo_eat(philo);

        if (philo->data->must_eat_count > 0
            && philo->eat_count >= philo->data->must_eat_count)
        {
            // printf("DEBUG: Philosopher %d finished %d meals\n", 
            //        philo->id, philo->eat_count);
            exit(0);
        }
        if (check_death_signal(philo))
            exit(0);

        // print_status(philo, "is sleeping");
		philo_think(philo);
        smart_sleep(philo->data->time_to_sleep, philo);
		if (check_death_signal(philo))
            exit(0);
    }
}
 
void	philo_eat(t_philo *philo)
{
	if (check_death_signal(philo))
        return;
	sem_wait(philo->data->forks);
	print_status(philo, "has taken a fork");
	if (check_death_signal(philo))
    {
        sem_post(philo->data->forks);
        return;
    }
	sem_wait(philo->data->forks);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal = get_current_time();
	pthread_mutex_unlock(&philo->meal_lock);
	print_status(philo, "is eating");
	philo->eat_count++;
	smart_sleep(philo->data->time_to_eat, philo);
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
}

void	smart_sleep(long long time, t_philo *philo)
{
	long long	start;

	start = get_current_time();
	while (get_current_time() - start < time)
	{
		if (get_current_time() - start >= time)
			break ;
		if (check_death_signal(philo))
            exit(0);
		usleep(100);
	}
}

void	philo_think(t_philo *philo)
{
	long long	think_time;
	long long	time_since_meal;
	long long	safe_margin;

	print_status(philo, "is thinking");
	time_since_meal = get_current_time() - philo->last_meal;
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
	smart_sleep(think_time, philo);
}

int check_death_signal(t_philo *philo)
{
    if (sem_trywait(philo->data->death) == 0)
    {
        sem_post(philo->data->death);
        return (1);
    }
    return (0);
}
