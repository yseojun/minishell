/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_run_unit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/03/14 13:35:49 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "base.h"
#include "util.h"

static void	run_unit(t_token *unit, t_pipe *info, t_data *data);
static int	run_single_builtin(t_pipe *info, t_data *data);
static void	child(t_pipe *info, t_data *data);
static void	run_command(t_pipe *info, t_data *data);
static int  chk_stat(char *path_command);

int	excute_tree(t_token *top, t_pipe *info, t_data *data)
{
	if (top == 0)
		return (1);
	if (top->type == AND)
	{
		if (excute_tree(top->left, info, data) == 1)
		{
			info->prev_fd = STDIN_FILENO;
			return (excute_tree(top->right, info, data));
		}
	}
	else if (top->type == OR)
	{
		if (excute_tree(top->left, info, data) == 0)
		{
			info->prev_fd = STDIN_FILENO;
			return (excute_tree(top->right, info, data));
		}
	}
	else if (top->type == PIPE)
	{
		info->is_pipe = TRUE;
		info->pipe_count++;
		if (top->left)
			excute_tree(top->left, info, data);
		info->is_pipe = TRUE;
		info->pipe_count--;
		if (top->right)
			excute_tree(top->right, info, data);
		info->is_pipe = FALSE;
		return (exit_status(LOAD) == SUCCESS);
	}
	else if (top->type == CMD || top->type == REDIRECTION)
	{
		free_arr((void **)info->cmd_arr);
		info->cmd_arr = 0;
		run_unit(top, info, data);
		return (exit_status(LOAD) == SUCCESS);
	}
	return (EXIT_SUCCESS);
}

static int	run_single_builtin(t_pipe *info, t_data *data)
{
	if (info->is_pipe || info->is_built_in == FALSE)
		return (FALSE);
	if (info->is_built_in == EXPORT && info->cmd_arr[1])
		exit_status(256 * builtin_export(data, info->cmd_arr));
	else if (info->is_built_in == UNSET)
		exit_status(256 * builtin_unset(data, info->cmd_arr));
	else if (info->is_built_in == EXIT)
		exit_status(256 * builtin_exit(info->cmd_arr));
	else if (info->is_built_in == CD)
		exit_status(256 * builtin_cd(data, info->cmd_arr[1]));
	else
		return (FALSE);
	return (TRUE);
}

static void	run_unit(t_token *unit, t_pipe *info, t_data *data)
{
	pid_t	pid;

	if (set_fd(unit, info) == FAILURE)
		return ;
	info->cmd_arr = set_cmd(unit);
	if (info->cmd_arr == 0)
		return ;
	info->is_built_in = 0;
	if (chk_cmd(info, data) == FAILURE)
		return ;
	if (run_single_builtin(info, data))
		return ;
	pid = _fork();
	if (pid == 0)
		child(info, data);
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, handler);
		add_pid(info, pid);
		if (info->in_fd != STDIN_FILENO)
			close(info->in_fd);
		if (info->out_fd != STDOUT_FILENO)
			close(info->out_fd);
		if (info->pipe_count == 0)
			wait_all(info);
		signal(SIGINT, handler);
		signal(SIGQUIT, SIG_IGN);
		info->prev_fd = STDIN_FILENO;
		if (info->pipe_count > 0)
			info->prev_fd = info->pipefd[P_READ];
	}
	return ;
}

static void	child(t_pipe *info, t_data *data)
{
	signal(SIGQUIT, SIG_DFL);
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
