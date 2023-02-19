/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:12:20 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/19 16:49:58 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe.h"
#include "base.h"
#include "util.h"

void	set_fd(t_pipe *info, int idx);

int	run_pipe(t_pipe *info, int idx)
{
	char	**tmp;
	pid_t	pid;

	tmp = ft_split(info->tmp[idx], ' ');
	// _transfrom(); "" '' $
	if (set_pipe(info, tmp) < 0)
		return (-1);
	_pipe(info->pipefd);
	set_fd(info, idx);
	pid = _fork();
	if (pid == 0)
		child(info);
	else
	{
		add_pid(info, pid);
		close(info->pipefd[1]);
		if (info->prev_fd != 0)
			close(info->prev_fd);
		info->prev_fd = info->pipefd[0];
	}
	return (0);
}

void	set_fd(t_pipe *info, int idx)
{
	info->in_fd = info->prev_fd;
	if (info->infile_fd != 0)
	{
		close(info->prev_fd);
		info->in_fd = info->infile_fd;
	}
	if (idx + 1 != info->tmp_size)
		info->out_fd = info->pipefd[1];
	else
	{
		close(info->pipefd[1]);
		info->out_fd = 1;
	}
	if (info->outfile_fd != 1)
	{
		close(info->pipefd[1]);
		info->out_fd = info->outfile_fd;
	}
}

void	child(t_pipe *info)
{
	printf("in : %d , out : %d\n", info->in_fd, info->out_fd);
	dup2(info->in_fd, 0);
	if (info->in_fd != 0)
		close(info->in_fd);
	dup2(info->out_fd, 1);
	if (info->out_fd != 1)
		close(info->out_fd);
	run_command(info);
}

void	run_command(t_pipe *info)
{
	char	*path_command;

	path_command = find_command_in_path(info->cmd[0], info->path);
	if (access(path_command, X_OK) == -1)
		perror_exit(info->cmd[0]);
	// pwd export cd unset env exit
	execve(path_command, info->cmd, environ);
	perror_exit(info->cmd[0]);
}

void	wait_all(t_pipe *info)
{
	int		status;
	t_pid	*search;

	search = info->pids;
	while (search)
	{
		waitpid(search->pid, &status, 0);
		search = search->next;
	}
}
