/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_run_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:12:20 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/26 14:12:04 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"

static void	child(t_pipe *info, t_data *data);
static void	run_command(t_pipe *info, t_data *data);

int	run_unit(t_pipe *info, t_data *data)
{
	pid_t	pid;

	prt_arr(info->unit); // 테스트 용도
	printf("%d\n", info->unit_size);
	_pipe(info->pipefd);
	set_fd(info);
	info->cmd_arr = set_cmd(info->unit);
	prt_arr(info->cmd_arr);
	chk_cmd(info); //to_do // pwd export cd unset env exit
	pid = _fork();
	if (pid == 0)
		child(info, data);
	else
	{
		add_pid(info, pid);
		close(info->pipefd[P_WRITE]); // 마지막 unit이 아닌 경우에만 닫아야 하지 않나?
		if (info->in_fd != STDIN_FILENO)
			close(info->in_fd);
		info->prev_fd = info->pipefd[P_READ];
	}
	return (SUCCESS);
}

static void	child(t_pipe *info, t_data *data)
{
	printf("in : %d , out : %d\n", info->in_fd, info->out_fd);
	dup2(info->in_fd, STDIN_FILENO);
	if (info->in_fd != STDIN_FILENO)
		close(info->in_fd);
	dup2(info->out_fd, STDOUT_FILENO);
	if (info->out_fd != STDOUT_FILENO)
		close(info->out_fd);
	run_command(info, data);
}

static void	run_command(t_pipe *info, t_data *data)
{
	char	*path_command;

	path_command = find_command_in_path(info->cmd_arr[0], info->path);
	// printf("path cmd: %s\n", path_command);
	if (access(path_command, X_OK) == FAILURE)
	{
		chk_user_func(info, data);
		return ;
	}
	else
		execve(path_command, info->cmd_arr, environ);
	perror_exit(info->cmd_arr[0]);
}
