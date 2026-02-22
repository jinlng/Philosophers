/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinliang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 18:34:53 by jinliang          #+#    #+#             */
/*   Updated: 2026/02/21 17:15:19 by jinliang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <semaphore.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>

# ifndef MAX_PHILOS
#  define MAX_PHILOS 250
# endif

typedef struct	s_philo
{
	int				id;
	int				eat_count;
	long long		last_meal;
	struct s_data	*data;
	pthread_t		monitor;
	pthread_mutex_t	meal_lock;
}	t_philo;

typedef struct s_data
{
	int			num_philos;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			must_eat_count;
	long long	start_time;
	sem_t		*forks;
	sem_t		*print;
	sem_t		*death;
	pid_t		*pids;
	int			is_parent;
	// sem_t *meal;
	t_philo 	*philos;
} t_data;

/* init */
int			init_data(t_data *data, int argc, char **argv);
int			init_semaphores(t_data *data);

/* main */
void		solo_philo(t_data *data);
void		start_simulation(t_data *data);
void 		print_status(t_philo *philo, char *status);

// void		*meal_monitor(void *arg);
// void		*death_monitor(void *arg);
// void		create_monitor_process(t_data *data);
void		*monitor_routine(void *arg);
// void monitor_process(t_data *data);
void		parent_monitor(t_data *data);
// void		wait_for_philosophers(t_data *data);
// int			all_philos_full(t_data *data);

// void		create_all_philosophers(t_data *data);
void		create_processes(t_data *data);
void		child_process(t_data *data, int index);
void		kill_all(t_data *data);

void		philo_routine(t_philo *philo);
// void		take_forks(t_philo *philo);
// void		release_forks(t_philo *philo);
void		philo_eat(t_philo *philo);
void		smart_sleep(long long time, t_philo *philo);
void	philo_think(t_philo *philo);
int check_death_signal(t_philo *philo);

void		clean_exit(t_data *data, int exit_code);
// void		kill_all(t_data *data);
// void		destroy_semaphores(t_data *data);
int			ft_atoi(const char *str);
long long	get_current_time(void);

#endif