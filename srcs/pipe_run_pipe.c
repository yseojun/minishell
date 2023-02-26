/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_run_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:12:20 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/26 12:45:58 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"

void		set_fd(t_pipe *info);
static void	child(t_pipe *info, t_data *data);
static void	run_command(t_pipe *info, t_data *data);

int	run_unit(t_pipe *info, t_data *data)
{
	pid_t	pid;

	prt_arr(info->unit);
	printf("%d\n", info->unit_size);
	info->infile_fd = 0;
	info->outfile_fd = 1;
	_pipe(info->pipefd);
	set_fd(info);
	info->cmd_arr = set_cmd(info->unit);
	chk_cmd(info); //to_do
	pid = _fork();
	if (pid == 0)
		child(info, data);
	else
	{
		add_pid(info, pid);
		close(info->pipefd[1]);
		if (info->in_fd != 0)
			close(info->in_fd);
		info->prev_fd = info->pipefd[0];
	}
	return (0);
}

// static void	set_fd(t_pipe *info)
// {
// 	info->in_fd = info->prev_fd;
// 	if (info->infile_fd != STDIN_FILENO)
// 	// if (is_infile())
// 	{
// 		close(info->prev_fd);
// 		info->in_fd = info->infile_fd;
// 	}
// 	if (is_pipe(info->unit[info->unit_size - 1]))
// 		info->out_fd = info->pipefd[1];
// 	else
// 	{
// 		close(info->pipefd[1]);
// 		info->out_fd = STDOUT_FILENO;
// 	}
// 	if (info->outfile_fd != STDOUT_FILENO)
// 	// if (is_outfile())
// 	{
// 		close(info->pipefd[1]);
// 		info->out_fd = info->outfile_fd;
// 	}
// }

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
