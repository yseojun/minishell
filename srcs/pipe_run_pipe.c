/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_run_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:12:20 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/25 16:55:08 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"

void		transform(char **tmp);
static void	set_fd(t_pipe *info, int idx);
static void	child(t_pipe *info, t_data *data);
static void	run_command(t_pipe *info, t_data *data);

int	run_pipe(t_pipe *info, t_data *data, int idx)
{
	char	**tmp;
	pid_t	pid;

	info->infile_fd = 0;
	info->outfile_fd = 1;
	// transform(tmp);
	_pipe(info->pipefd);
	set_fd(info, idx);
	chk_cmd(info); //to_do
	pid = _fork();
	if (pid == 0)
		child(info, data);
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

static void	set_fd(t_pipe *info, int idx)
{
	info->in_fd = info->prev_fd;
	// 이전 파이프의 출구, 맨 처음엔 prev_fd를 0으로 세팅
	// if (info->infile_fd != STDIN_FILENO)
	if (is_infile())
	{
		close(info->prev_fd);
		info->in_fd = info->infile_fd;
	}
	if (idx + 1 != info->unit_count)
		info->out_fd = info->pipefd[1];
	else
	{
		close(info->pipefd[1]);
		info->out_fd = STDOUT_FILENO;
	}
	// if (info->outfile_fd != STDOUT_FILENO)
	if (is_outfile())
	{
		close(info->pipefd[1]);
		info->out_fd = info->outfile_fd;
	}
}

static void	child(t_pipe *info, t_data *data)
{
	printf("in : %d , out : %d\n", info->in_fd, info->out_fd);
	dup2(info->in_fd, 0);
	if (info->in_fd != 0)
		close(info->in_fd);
	dup2(info->out_fd, 1);
	if (info->out_fd != 1)
		close(info->out_fd);
	run_command(info, data);
}

static void	run_command(t_pipe *info, t_data *data)
{
	char	*path_command;

	path_command = find_command_in_path(info->cmd_arr[0], info->path);
	// printf("path cmd: %s\n", path_command);
	if (access(path_command, X_OK) == -1)
	{
		chk_user_func(info, data);
		// pwd export cd unset env exit
		return ;
	}
	else
		execve(path_command, info->cmd_arr, environ);
	perror_exit(info->cmd_arr[0]);
}
