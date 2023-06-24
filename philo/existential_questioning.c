/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   existential_questioning.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberard <vberard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 18:21:18 by vberard           #+#    #+#             */
/*   Updated: 2023/05/22 21:04:42 by vberard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	left_fork_assign(int *l_p, int i, int p_count)
{
	if (p_count == 1)
		return (0);
	if (i == 0)
		l_p[0] = p_count - 1;
	else if (i == p_count - 1)
		l_p[0] = i - 1;
	else
		l_p[0] = i - 1;
	return (1);
}

int	should_i_die(t_simu *s, int i, struct timeval eat_st)
{
	struct timeval	l_time;

	pthread_mutex_lock(&s->d_mutex[i]);
	if (s->d[i])
	{
		pthread_mutex_unlock(&s->d_mutex[i]);
		return (1);
	}
	pthread_mutex_unlock(&s->d_mutex[i]);
	gettimeofday(&l_time, NULL);
	if (((l_time.tv_sec * 1000) + (l_time.tv_usec / 1000))
		- ((eat_st.tv_sec * 1000) + (eat_st.tv_usec / 1000)) >= s->tt_d)
	{
		kallm(s, i);
		return (1);
	}
	return (0);
}

int	can_i_wake_up(t_simu *s, int i, struct timeval sleep_st)
{
	struct timeval	l_time;
	int				l_p;

	if (!left_fork_assign(&l_p, i, s->p_count))
		return (0);
	usleep(10);
	pthread_mutex_lock(&s->d_mutex[i]);
	if (s->d[i])
	{
		pthread_mutex_unlock(&s->d_mutex[i]);
		return (4);
	}
	pthread_mutex_unlock(&s->d_mutex[i]);
	gettimeofday(&l_time, NULL);
	if (((l_time.tv_sec * 1000) + (l_time.tv_usec / 1000))
		- ((sleep_st.tv_sec * 1000) + (sleep_st.tv_usec / 1000)) >= s->tt_s)
		return (1);
	return (0);
}

int	can_i_stop_eating(t_simu *s, int i, struct timeval eat_st)
{
	struct timeval	l_time;
	int				l_p;

	if (!left_fork_assign(&l_p, i, s->p_count))
		return (0);
	usleep(10);
	pthread_mutex_lock(&s->d_mutex[i]);
	if (s->d[i])
	{
		pthread_mutex_unlock(&s->d_mutex[i]);
		return (4);
	}
	gettimeofday(&l_time, NULL);
	if (((l_time.tv_sec * 1000) + (l_time.tv_usec / 1000))
		- ((eat_st.tv_sec * 1000) + (eat_st.tv_usec / 1000)) >= s->tt_e)
	{
		l2m(&s->i_mutex[i], &s->i_mutex[l_p], i);
		s->p_status[l_p] = 0;
		s->p_status[i] = 0;
		mutex_u3(&s->i_mutex[l_p], &s->i_mutex[i], &s->d_mutex[i]);
		return (1);
	}
	pthread_mutex_unlock(&s->d_mutex[i]);
	return (0);
}

int	can_i_eat(t_simu *s, int i, int *eatc, struct timeval *eat_st)
{
	int	l_p;

	if (!left_fork_assign(&l_p, i, s->p_count))
		return (0);
	pthread_mutex_lock(&s->d_mutex[i]);
	if (s->d[i])
	{
		pthread_mutex_unlock(&s->d_mutex[i]);
		return (4);
	}
	l2m(&s->i_mutex[i], &s->i_mutex[l_p], i);
	if (s->p_status[l_p] == 1 || s->p_status[i] == 1)
	{
		mutex_u3(&s->i_mutex[l_p], &s->i_mutex[i], &s->d_mutex[i]);
		return (0);
	}
	pick_up_forks(i, l_p, s);
	gettimeofday(&eat_st[0], NULL);
	pthread_mutex_lock(&s->p_mutex);
	printf("%zu %d is eating\n", t_ms(s), i + 1);
	pthread_mutex_unlock(&s->p_mutex);
	eatc[0]++;
	mutex_u3(&s->i_mutex[l_p], &s->i_mutex[i], &s->d_mutex[i]);
	return (1);
}
