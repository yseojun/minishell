/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _util.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 14:28:00 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/26 14:04:42 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"
#include "base.h"

void	_pipe(int *pipefd)
{
	if (pipe(pipefd) == FAILURE)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}

pid_t	_fork(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == FAILURE)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	return (pid);
}

void	_dup2(int fd1, int fd2)
{
	if (dup2(fd1, fd2) == FAILURE)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
}

void	perror_exit(char *name)
{
	perror(name);
	exit(EXIT_FAILURE);
}
