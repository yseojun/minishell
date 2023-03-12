/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_run_unit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/03/12 14:02:34 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "base.h"
#include "util.h"

static void	child(t_pipe *info, t_data *data);
static void	run_command(t_pipe *info, t_data *data);
static int  chk_stat(char *path_command);

int	excute_tree(t_token *top, t_pipe *info, t_data *data)
{
	if (top->type == AND)
	{
		info->is_pipe = 0;
		if (excute_tree(top->left, info, data) == 1)
			return (excute_tree(top->right, info, data));
	}
	else if (top->type == OR)
	{
		info->is_pipe = 0;
		if (excute_tree(top->left, info, data) == 0)
			return (excute_tree(top->right, info, data));
	}
	else if (top->type == PIPE)
	{
		info->is_pipe = 1;
		info->pipe_count++;
		excute_tree(top->left, info, data);
		excute_tree(top->right, info, data);
		return (data->exit_status == SUCCESS);
	}
	else if (top->type == CMD || top->type == REDIRECTION)
	{
		free_arr((void **)info->cmd_arr);
		return (run_unit(top, info, data) == SUCCESS);
	}
	return (0);
}

int	run_single_builtin(t_pipe *info, t_data *data)
{
	if (info->is_pipe || info->is_built_in == FALSE)
		return (FALSE);
	if (info->is_built_in == EXPORT && info->cmd_arr[1])
		data->exit_status = builtin_export(data, info->cmd_arr);
	else if (info->is_built_in == UNSET)
		data->exit_status = builtin_unset(data, info->cmd_arr);
	else if (info->is_built_in == EXIT)
		data->exit_status = builtin_exit(info->cmd_arr);
	else if (info->is_built_in == CD)
		data->exit_status = builtin_cd(info->cmd_arr[1]);
	else
		return (FALSE);
	return (TRUE);
}

int	run_unit(t_token *unit, t_pipe *info, t_data *data)
{
	pid_t	pid;

	if (set_fd(unit, info) == FAILURE)
		return (FAILURE);
	info->cmd_arr = set_cmd(unit);
	if (info->cmd_arr == 0)
		return (FAILURE);
	info->is_built_in = 0;
	if (chk_cmd(info, data) == FAILURE)
		return (FAILURE);
	if (run_single_builtin(info, data))
		return (data->exit_status);
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
		if (info->pipe_count == 0)
			wait_all(info, data);
		info->prev_fd = STDIN_FILENO;
		if (info->pipe_count > 0)
		{
			info->pipe_count--;
			info->prev_fd = info->pipefd[P_READ];
		}
	}
	return (data->exit_status);
}

static void	child(t_pipe *info, t_data *data)
{
	//printf("in : %d , out : %d\n", info->in_fd, info->out_fd);
	if  (info->pipe_count > 0)
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
		exit(run_builtin_func(info, data));
	if (info->cmd_arr)
		path_command = find_command_in_path(info->cmd_arr[0], data);
	if (access(path_command, X_OK) == SUCCESS)
	{
		if (chk_stat(path_command) == SUCCESS)
			execve(path_command, info->cmd_arr, NULL);
		else
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(info->cmd_arr[0], 2);
			ft_putstr_fd(": ", 2);
			ft_putendl_fd(strerror(21), 2);
			exit(126);	
		}
	}
	perror_exit(info->cmd_arr[0]);
}

static int  chk_stat(char *path_command)
{
    struct stat sp;
    // 반환값에 따른 에러처리를 해줘야하는 지는 잘 모르겠다
    lstat(path_command, &sp);
    if (S_ISDIR(sp.st_mode))
        return (FAILURE);
    return (SUCCESS);
}
