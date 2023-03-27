/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _util.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 14:28:00 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/26 16:34:45 by seojyang         ###   ########.fr       */
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
		printf("fd1: %d, fd2: %d\n", fd1, fd2);
		perror("dup2");
		exit(EXIT_FAILURE);
	}
}

void	free_arr(void **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
	arr = 0;
	return ;
}
