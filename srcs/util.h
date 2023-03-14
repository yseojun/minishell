/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 14:28:00 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/14 16:53:41 by rolee            ###   ########.fr       */
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
void	free_arr(void **arr);

#endif