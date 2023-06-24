/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberard <vberard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 18:46:09 by vberard           #+#    #+#             */
/*   Updated: 2023/05/22 20:38:31 by vberard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_deploy(t_simu *s)
{
	int	i;

	i = 0;
	p_deploy(s);
	while (i < s->p_count)
	{
		pthread_mutex_destroy(&s->i_mutex[i]);
		pthread_mutex_destroy(&s->d_mutex[i]);
		i++;
	}
	pthread_mutex_destroy(&s->p_mutex);
	free(s->threads);
	free(s->p_status);
	free(s->i_mutex);
	free(s->d);
	free(s->d_mutex);
	return (0);
}

int	init_everything(t_simu *s)
{
	int	i;

	i = 0;
	if (pthread_mutex_init(&s->p_mutex, NULL) != 0 || init_death(s))
	{
		free(s->threads);
		free(s->p_status);
		free(s->i_mutex);
		return (1);
	}
	while (i < s->p_count)
	{
		if (pthread_mutex_init(&s->i_mutex[i], NULL) != 0)
		{
			while (i-- + 1 < 0)
				pthread_mutex_destroy(&s->i_mutex[i - 1]);
			pthread_mutex_destroy(&s->p_mutex);
			free(s->threads);
			free(s->p_status);
			free(s->i_mutex);
			return (1);
		}
		i++;
	}
	return (init_deploy(s));
}

int	parse_and_assign(int ac, char **av, t_simu *s)
{
	if (ac < 5)
	{
		printf("Not enough args\n");
		return (0);
	}
	if (ac > 6)
	{
		printf("Too many args\n");
		return (0);
	}
	s->p_me = -1;
	s->p_count = ft_atoi(av[1]);
	s->tt_d = ft_atoi(av[2]);
	s->tt_e = ft_atoi(av[3]);
	s->tt_s = ft_atoi(av[4]);
	if (ac == 6)
		s->p_me = ft_atoi(av[5]);
	if (s->p_me < -1 || s->p_count < -1 || s->tt_d < -1
		|| s->tt_e < -1 || s->tt_s < -1)
	{
		printf("Bad args\n");
		return (0);
	}
	return (1);
}

int	main(int ac, char **av)
{
	t_simu	s;

	if (!parse_and_assign(ac, av, &s))
		return (1);
	s.threads = (pthread_t *)malloc((s.p_count + 1) * sizeof(pthread_t));
	if (!s.threads)
		return (1);
	s.i_mutex = (pthread_mutex_t *)malloc((s.p_count + 1)
			* sizeof(pthread_mutex_t));
	if (!s.i_mutex)
	{
		free (s.threads);
		return (1);
	}
	s.p_status = (int *)malloc((s.p_count + 1) * sizeof(int));
	if (!s.p_status)
	{
		free(s.threads);
		free(s.i_mutex);
		return (1);
	}
	ft_bzero(s.p_status, (s.p_count + 1) * sizeof(int));
	ft_bzero(s.threads, (s.p_count + 1) * sizeof(pthread_t));
	ft_bzero(s.i_mutex, (s.p_count + 1) * sizeof(pthread_mutex_t));
	return (init_everything(&s));
}
