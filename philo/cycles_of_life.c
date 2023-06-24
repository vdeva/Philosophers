/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycles_of_life.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberard <vberard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 19:06:40 by vberard           #+#    #+#             */
/*   Updated: 2023/05/22 21:36:15 by vberard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	cycle3(t_is *is, struct timeval *eat_st, struct timeval *sleep_st)
{
	int	sst;

	sst = 0;
	while (!sst)
	{
		usleep(1);
		if (should_i_die(is->s, is->id - 1, eat_st[0]))
			return (0);
		sst = can_i_wake_up(is->s, is->id - 1, sleep_st[0]);
		if (sst == 4)
			return (0);
	}
	if ((is->id - 1) % 2 != 0)
		usleep(1000);
	return (1);
}

int	cycle2(t_is *is, struct timeval *eat_st, struct timeval *sleep_st)
{
	int	sst;

	sst = 0;
	while (!sst)
	{
		usleep(1);
		if (should_i_die(is->s, is->id - 1, eat_st[0]))
			return (0);
		sst = can_i_stop_eating(is->s, is->id - 1, eat_st[0]);
		if (sst == 4)
			return (0);
	}
	gettimeofday(sleep_st, NULL);
	if (should_i_die(is->s, is->id - 1, eat_st[0]))
		return (0);
	pthread_mutex_lock(&is->s->p_mutex);
	printf("%zu %d is sleeping\n", t_ms(is->s), is->id);
	pthread_mutex_unlock(&is->s->p_mutex);
	return (1);
}

int	cycle1(t_is *is, int *eatc, struct timeval *eat_st)
{
	int	sst;

	sst = 0;
	pthread_mutex_lock(&is->s->d_mutex[is->id - 1]);
	if (is->s->d[is->id - 1])
	{
		pthread_mutex_unlock(&is->s->d_mutex[is->id - 1]);
		return (0);
	}
	pthread_mutex_lock(&is->s->p_mutex);
	printf("%zu %d is thinking\n", t_ms(is->s), is->id);
	pthread_mutex_unlock(&is->s->p_mutex);
	pthread_mutex_unlock(&is->s->d_mutex[is->id - 1]);
	while (!sst)
	{
		usleep(100);
		if (should_i_die(is->s, is->id - 1, eat_st[0]))
			return (0);
		sst = can_i_eat(is->s, is->id - 1, eatc, eat_st);
		if (sst == 4)
			return (0);
	}
	return (1);
}

void	*p_spawn(void *ptr)
{
	t_is			*is;
	struct timeval	sleep_st;
	struct timeval	eat_st;
	int				eatc;

	is = ptr;
	eatc = 0;
	eat_st.tv_sec = is->s->u_time.tv_sec;
	eat_st.tv_usec = is->s->u_time.tv_usec;
	if ((is->id - 1) % 2 == 0)
		usleep(1000);
	while ((is->s->p_me == -1 || eatc < is->s->p_me))
	{
		if (cycle1(is, &eatc, &eat_st) == 0)
			return (NULL);
		if (cycle2(is, &eat_st, &sleep_st) == 0)
			return (NULL);
		if (cycle3(is, &eat_st, &sleep_st) == 0)
			return (NULL);
	}
	return (NULL);
}

void	p_deploy(t_simu *s)
{
	t_is	*is;
	int		i;

	gettimeofday(&s->u_time, NULL);
	is = (t_is *)malloc((sizeof(t_is) * s->p_count));
	if (!is)
		return ;
	i = 1;
	while (i - 1 < s->p_count)
	{
		is[i - 1].id = i;
		is[i - 1].s = s;
		pthread_create(&s->threads[i - 1], NULL, p_spawn, &is[i - 1]);
		i++;
	}
	i = 1;
	while (i - 1 < s->p_count)
	{
		pthread_join(s->threads[i - 1], NULL);
		i++;
	}
	free(is);
}
