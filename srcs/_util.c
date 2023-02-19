/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _util.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 14:28:00 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/19 16:53:03 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"

void	_pipe(int *pipefd)
{
	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		exit(1);
	}
}

pid_t	_fork(void)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		exit(1);
	}
	return (pid);
}

void	_dup2(int fd1, int fd2)
{
	if (dup2(fd1, fd2) < 0)
	{
		perror("dup2");
		exit(1);
	}
}

void	perror_exit(char *name)
{
	perror(name);
	exit(1);
}
