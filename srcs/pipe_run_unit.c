/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_run_unit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/03/14 20:49:44 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "base.h"
#include "util.h"

static void	run_unit(t_token *unit, t_data *data);
static int	run_single_builtin(t_data *data);
static void	child(t_data *data);
static void	run_command(t_data *data);
static int  chk_stat(char *path_command);

int	excute_tree(t_token *top, t_data *data)
{
	if (top == 0)
		return (1);
	if (top->type == AND)
	{
		if (excute_tree(top->left, data) == 1)
		{
			data->prev_fd = STDIN_FILENO;
			return (excute_tree(top->right, data));
		}
	}
	else if (top->type == OR)
	{
		if (excute_tree(top->left, data) == 0)
		{
			data->prev_fd = STDIN_FILENO;
			return (excute_tree(top->right, data));
		}
	}
	else if (top->type == PIPE)
	{
		data->is_pipe = TRUE;
		data->pipe_count++;
		if (top->left)
			excute_tree(top->left, data);
		data->is_pipe = TRUE;
		data->pipe_count--;
		if (top->right)
			excute_tree(top->right, data);
		data->is_pipe = FALSE;
		return (exit_status(LOAD) == SUCCESS);
	}
	else if (top->type == CMD || top->type == REDIRECTION)
	{
		free_arr((void **)data->cmd_arr);
		data->cmd_arr = 0;
		run_unit(top, data);
		return (exit_status(LOAD) == SUCCESS);
	}
	return (EXIT_SUCCESS);
}

static int	run_single_builtin(t_data *data)
{
	if (data->is_pipe || data->is_built_in == FALSE)
		return (FALSE);
	if (data->is_built_in == EXPORT && data->cmd_arr[1])
		exit_status(builtin_export(data, data->cmd_arr));
	else if (data->is_built_in == UNSET)
		exit_status(builtin_unset(data, data->cmd_arr));
	else if (data->is_built_in == EXIT)
		exit_status(builtin_exit(data->cmd_arr));
	else if (data->is_built_in == CD)
		exit_status(builtin_cd(data, data->cmd_arr[1]));
	else
		return (FALSE);
	return (TRUE);
}

static void	run_unit(t_token *unit, t_data *data)
{
	pid_t	pid;

	if (set_fd(unit, data) == FAILURE)
		return ;
	data->cmd_arr = set_cmd(unit);
	if (data->cmd_arr == 0)
		return ;
	data->is_built_in = 0;
	if (chk_cmd(data) == FAILURE)
		return ;
	if (run_single_builtin(data))
		return ;
	pid = _fork();
	if (pid == 0)
		child(data);
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, handler);
		add_pid(data, pid);
		if (data->in_fd != STDIN_FILENO)
			close(data->in_fd);
		if (data->out_fd != STDOUT_FILENO)
			close(data->out_fd);
		if (data->pipe_count == 0)
			wait_all(data);
		signal(SIGINT, handler);
		signal(SIGQUIT, SIG_IGN);
		data->prev_fd = STDIN_FILENO;
		if (data->pipe_count > 0)
			data->prev_fd = data->pipefd[P_READ];
	}
	return ;
}

static void	child(t_data *data)
{
	signal(SIGQUIT, SIG_DFL);
	if (data->pipe_count > 0)
		close(data->pipefd[P_READ]);
	dup2(data->in_fd, STDIN_FILENO);
	if (data->in_fd != STDIN_FILENO)
		close(data->in_fd);
	dup2(data->out_fd, STDOUT_FILENO);
	if (data->out_fd != STDOUT_FILENO)
		close(data->out_fd);
	run_command(data);
}

static void	run_command(t_data *data)
{
	char	*path_command;

	if (data->is_built_in)
		exit(run_builtin_func(data));
	if (data->cmd_arr)
		path_command = find_command_in_path(data->cmd_arr[0], data);
	if (access(path_command, X_OK) == SUCCESS)
	{
		if (chk_stat(path_command) == SUCCESS)
			execve(path_command, data->cmd_arr, NULL);
		else
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(data->cmd_arr[0], 2);
			ft_putstr_fd(": ", 2);
			ft_putendl_fd(strerror(21), 2);
			exit(126);
		}
	}
	perror_exit(data->cmd_arr[0]);
}

static int	chk_stat(char *path_command)
{
	struct stat	sp;

	lstat(path_command, &sp);
	if (S_ISDIR(sp.st_mode))
		return (FAILURE);
	return (SUCCESS);
}
