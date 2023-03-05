/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_run_unit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:12:20 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/05 17:50:41 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"

static void	child(t_pipe *info, t_data *data);
static void	run_command(t_pipe *info, t_data *data);

// int excute_tree(t_token *top, t_pipe *info, t_data *data)
// {
// 	if (top->type == AND)
// 		if (excute_tree(top->left, info, data))
// 			excute_tree(top->right, info, data);
// 	else if (top->type == CMD || top->type == REDIRECTION)
// 		run_unit(top, info, data);
// }

int	run_unit(t_token *unit, t_pipe *info, t_data *data)
{
	pid_t	pid;

	if (set_fd(info) == FAILURE)
		return (FAILURE);
	info->cmd_arr = set_cmd(info->unit);
	if (info->cmd_arr != 0 && chk_cmd(info, data) == FAILURE)
		return (FAILURE);
	// printf("%d\n", info->is_built_in);
	pid = _fork();
	if (pid == 0)
		child(info, data);
	else
	{
		add_pid(info, pid);
		if (info->is_pipe > 0)
		{
			info->is_pipe--;
			close(info->pipefd[P_WRITE]);
			info->prev_fd = info->pipefd[P_READ];
		}
		if (info->in_fd != STDIN_FILENO)
			close(info->in_fd);
		if (info->out_fd != STDOUT_FILENO)
			close(info->out_fd);
	}
	return (SUCCESS);
}

static void	child(t_pipe *info, t_data *data)
{
	// printf("in : %d , out : %d\n", info->in_fd, info->out_fd);
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

	if (info->is_built_in)
	{
		run_builtin_func(info, data);
		return ;
	}
	if (info->cmd_arr)
		path_command = find_command_in_path(info->cmd_arr[0], data);
	//printf("path cmd: %s\n", path_command);
	if (access(path_command, X_OK) == FAILURE)
		return ;
	else
		execve(path_command, info->cmd_arr, environ);
	perror_exit(info->cmd_arr[0]);
}
