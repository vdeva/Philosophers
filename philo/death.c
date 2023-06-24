/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberard <vberard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 18:11:42 by vberard           #+#    #+#             */
/*   Updated: 2023/05/22 21:03:45 by vberard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	pick_up_forks(int i, int l_p, t_simu *s)
{
	s->p_status[l_p] = 1;
	pthread_mutex_lock(&s->p_mutex);
	printf("%zu %d has taken a fork\n", t_ms(s), i + 1);
	pthread_mutex_unlock(&s->p_mutex);
	s->p_status[i] = 1;
	pthread_mutex_lock(&s->p_mutex);
	printf("%zu %d has taken a fork\n", t_ms(s), i + 1);
	pthread_mutex_unlock(&s->p_mutex);
	return ;
}

void	kallm(t_simu *s, int id)
{
	int	i;

	i = 0;
	while (i < s->p_count)
	{
		pthread_mutex_lock(&s->d_mutex[i]);
		i++;
	}
	i = 0;
	if (s->d[0] != 1)
	{
		printf("%zu %d died\n", t_ms(s), id + 1);
		while (i < s->p_count)
		{
			s->d[i] = 1;
			i++;
		}
	}
	i = 0;
	while (i < s->p_count)
	{
		pthread_mutex_unlock(&s->d_mutex[i]);
		i++;
	}
	return ;
}

int	death_spiral(t_simu *s, int i)
{
	while (i < s->p_count)
	{
		if (pthread_mutex_init(&s->d_mutex[i], NULL) != 0)
		{
			while (i < 0)
			{
				pthread_mutex_destroy(&s->d_mutex[i - 1]);
				i--;
			}
			free(s->d);
			free(s->d_mutex);
			return (1);
		}
		i++;
	}
	return (0);
}

int	init_death(t_simu *s)
{
	int	i;

	i = 0;
	s->d_mutex = (pthread_mutex_t *)malloc((s->p_count + 1)
			* sizeof(pthread_mutex_t));
	if (!s->d_mutex)
		return (1);
	s->d = (int *)malloc((s->p_count + 1) * sizeof(int));
	if (!s->d)
	{
		free(s->d_mutex);
		return (1);
	}
	ft_bzero(s->d, (s->p_count + 1) * sizeof(int));
	ft_bzero(s->d_mutex, (s->p_count + 1) * sizeof(pthread_mutex_t));
	return (death_spiral(s, i));
}
