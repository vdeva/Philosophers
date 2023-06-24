/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberard <vberard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 18:51:39 by vberard           #+#    #+#             */
/*   Updated: 2023/05/22 20:59:24 by vberard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	unsigned int	i;
	unsigned int	res;

	i = 0;
	res = 0;
	if (!(str[i] > 47 && str[i] < 58))
		return (-2);
	while (str[i] > 47 && str[i] < 58)
	{
		res = res * 10 + (str[i] - 48);
		i++;
	}
	if (str[i] != '\0')
		return (-2);
	if (res > 2147483647)
		return (-2);
	return (res);
}

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*str;

	str = (unsigned char *)s;
	while (n-- > 0)
		str[n] = '\0';
}

size_t	t_ms(t_simu *s)
{
	struct timeval	l_time;

	gettimeofday(&l_time, NULL);
	return (((l_time.tv_sec * 1000) + (l_time.tv_usec / 1000))
		- ((s->u_time.tv_sec * 1000) + (s->u_time.tv_usec / 1000)));
}

void	l2m(pthread_mutex_t *m1, pthread_mutex_t *m2, int i)
{
	if (i % 2 == 0)
	{
		pthread_mutex_lock(m1);
		pthread_mutex_lock(m2);
		return ;
	}
	pthread_mutex_lock(m2);
	pthread_mutex_lock(m1);
	return ;
}

void	mutex_u3(pthread_mutex_t *m1, pthread_mutex_t *m2, pthread_mutex_t *m3)
{
	pthread_mutex_unlock(m1);
	pthread_mutex_unlock(m2);
	pthread_mutex_unlock(m3);
}
