/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinliang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 10:53:14 by jinliang          #+#    #+#             */
/*   Updated: 2026/01/12 15:04:44 by jinliang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
		return (print_error("Error: Usage: ./philo number_of_philosophers \
			time_to_die time_to_eat time_to_sleep [must_eat_count]"));
	if (!init_data(&data, argc, argv))
		return (print_error("Error: Initialization failed"));
	start_simulation(&data);
	cleanup(&data);
	return (0);
}

int	print_error(char *str)
{
	printf("%s\n", str);
	return (1);
}
