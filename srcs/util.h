/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 14:28:00 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/26 14:22:27 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_H
# define UTIL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>

// util.c
void	_pipe(int *pipefd);
pid_t	_fork(void);
void	_dup2(int fd1, int fd2);
void	perror_exit(char *name);

// util_arr.c
int		free_arr(void **arr);
int		chk_arr_size(char **tmp);
void	prt_arr(char **arr); //테스트 용도. 마지막에 지우자

#endif