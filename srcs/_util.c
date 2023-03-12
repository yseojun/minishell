/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _util.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 14:28:00 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/12 19:56:46 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"
#include "base.h"

void	_pipe(int *pipefd)
{
	if (pipe(pipefd) == FAILURE)
	{
		perror("pipe");
		exit(MY_EXIT_FAILURE);
	}
}

pid_t	_fork(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == FAILURE)
	{
		perror("fork");
		exit(MY_EXIT_FAILURE);
	}
	return (pid);
}

void	_dup2(int fd1, int fd2)
{
	if (dup2(fd1, fd2) == FAILURE)
	{
		perror("dup2");
		exit(MY_EXIT_FAILURE);
	}
}

void	perror_exit(char *name)
{
	ft_putstr_fd("minishell: ", 2);
	perror(name);
	exit(MY_EXIT_FAILURE);
}
