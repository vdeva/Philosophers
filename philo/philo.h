/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vberard <vberard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 16:46:50 by vberard           #+#    #+#             */
/*   Updated: 2023/05/22 21:03:59 by vberard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>			// for printf()
# include <string.h>		// for memset()
# include <stdlib.h>		// for malloc() and free()
# include <unistd.h>		// for write() and usleep()
# include <sys/time.h>		// for gettimeofday()
# include <pthread.h>		// for pthread functions
# include <stdint.h>		// for int stuff

typedef struct s_simu
{
	int				p_count;
	int				tt_d;
	int				tt_e;
	int				tt_s;
	int				p_me;
	int				*p_status;
	struct timeval	u_time;
	pthread_mutex_t	p_mutex;
	pthread_mutex_t	*i_mutex;
	pthread_t		*threads;
	int				*d;
	pthread_mutex_t	*d_mutex;
}	t_simu;

typedef struct s_is
{
	int		id;
	t_simu	*s;
}	t_is;

// deploy
void	p_deploy(t_simu *s);

// utils
size_t	t_ms(t_simu *s);
int		ft_atoi(const char *str);
void	ft_bzero(void *s, size_t n);
void	l2m(pthread_mutex_t *m1, pthread_mutex_t *m2, int i);
void	mutex_u3(pthread_mutex_t *m1, pthread_mutex_t *m2, pthread_mutex_t *m3);
// i put this one in death.c due to 42's file restrictions
void	pick_up_forks(int i, int l_p, t_simu *s);

// existential questions
int		should_i_die(t_simu *s, int i, struct timeval eat_st);
int		can_i_wake_up(t_simu *s, int i, struct timeval sleep_st);
int		can_i_stop_eating(t_simu *s, int i, struct timeval eat_st);
int		can_i_eat(t_simu *s, int i, int *eatc, struct timeval *eat_st);

//	death
int		init_death(t_simu *s);
void	kallm(t_simu *s, int id);

#endif