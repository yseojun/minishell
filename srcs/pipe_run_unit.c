/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_run_unit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:12:20 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/11 15:44:58 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"

static void	child(t_pipe *info, t_data *data);
static void	run_command(t_pipe *info, t_data *data);

int	excute_tree(t_token *top, t_pipe *info, t_data *data)
{
	if (top->type == AND)
	{
		if (excute_tree(top->left, info, data) > 0)
			return (excute_tree(top->right, info, data));
	}
	else if (top->type == OR)
	{
		if (excute_tree(top->left, info, data) == 0)
			return (excute_tree(top->right, info, data));
	}
	else if (top->type == PIPE)
	{
		info->is_pipe++;
		excute_tree(top->left, info, data);
		excute_tree(top->right, info, data);
		return (data->exit_status == 0);
	}
	else if (top->type == CMD || top->type == REDIRECTION)
		return (run_unit(top, info, data));
	return (0);
}

int	run_unit(t_token *unit, t_pipe *info, t_data *data)
{
	pid_t	pid;

	if (set_fd(unit, info) == FAILURE)
		return (FAILURE);
	info->cmd_arr = set_cmd(unit);
	info->is_built_in = 0;
	if (info->cmd_arr != 0 && chk_cmd(info, data) == FAILURE)
		return (FAILURE);
	// printf("%d\n", info->is_built_in);
	pid = _fork();
	if (pid == 0)
		child(info, data);
	else
	{
		add_pid(info, pid);
		if (info->in_fd != STDIN_FILENO)
			close(info->in_fd);
		if (info->out_fd != STDOUT_FILENO)
			close(info->out_fd);
		if (info->is_pipe == 0)
			wait_all(info, data);
		info->prev_fd = STDIN_FILENO;
		if (info->is_pipe > 0)
		{
			info->is_pipe--;
			info->prev_fd = info->pipefd[P_READ];
		}
	}
	return (SUCCESS);
}

static void	child(t_pipe *info, t_data *data)
{
	//printf("in : %d , out : %d\n", info->in_fd, info->out_fd);
	if  (info->is_pipe > 0)
		close(info->pipefd[P_READ]);
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
		exit(EXIT_FAILURE); // 임시
	}
	if (info->cmd_arr)
		path_command = find_command_in_path(info->cmd_arr[0], data);
	//printf("path cmd: %s\n", path_command);
	if (access(path_command, X_OK) == FAILURE)
		exit(EXIT_FAILURE);
	else
		execve(path_command, info->cmd_arr, environ);
	perror_exit(info->cmd_arr[0]);
}
