/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execute_tree.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 16:25:31 by rolee             #+#    #+#             */
/*   Updated: 2023/03/25 19:35:08 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"
#include "parse.h"

static int	execute_and(t_token *top, t_data *data);
static int	execute_or(t_token *top, t_data *data);
static int	execute_pipe(t_token *top, t_data *data);
static int	execute_brace(t_token *top, t_data *data);

int	execute_tree(t_token *top, t_data *data)
{
	if (top == NULL)
		return (TRUE);
	if (top->type == BRACE)
		return (execute_brace(top, data));
	if (top->type == AND)
		return (execute_and(top, data));
	else if (top->type == OR)
		return (execute_or(top, data));
	else if (top->type == PIPE)
		return (execute_pipe(top, data));
	else
	{
		free_arr((void **)data->cmd_arr);
		data->cmd_arr = 0;
		if (transform(data, top) == FAILURE)
			return (FAILURE);
		run_unit(top, data);
		if (data->cmd_arr && ft_strncmp(data->cmd_arr[0], "exit", 5) == 0)
			data->is_exit = 1;
		return (exit_status(LOAD) == SUCCESS);
	}
}

static int	execute_brace(t_token *top, t_data *data)
{
	pid_t	pid;

	pid = _fork();
	if (pid == 0)
	{
		data->pids = 0;
		data->pipe_count = 0;
		data->is_pipe = 0;
		data->cmd_count = 0;
		execute_tree(top->left, data);
		exit(exit_status(LOAD));
	}
	add_pid(data, pid);
	if (data->pipe_count == 0)
	{
		if (data->prev_fd != STDIN_FILENO)
			close(data->prev_fd);
		wait_all(data);
	}
	return (exit_status(LOAD) == SUCCESS);
}

static int	execute_and(t_token *top, t_data *data)
{
	int	status;

	data->cmd_count++;
	status = execute_tree(top->left, data);
	data->cmd_count--;
	if (status == TRUE && data->is_exit == 0)
		return (execute_tree(top->right, data));
	return (FALSE);
}

static int	execute_or(t_token *top, t_data *data)
{
	int	status;

	data->cmd_count++;
	status = execute_tree(top->left, data);
	data->cmd_count--;
	if (status == FALSE && data->is_exit == 0)
		return (execute_tree(top->right, data));
	return (TRUE);
}

static int	execute_pipe(t_token *top, t_data *data)
{
	int	pipefd[2];

	_pipe(pipefd);
	lst_pipefd_add_back(&data->listfd, lst_new_pipefd(pipefd));
	data->is_exit = 0;
	data->is_pipe = TRUE;
	data->pipe_count++;
	execute_tree(top->left, data);
	data->prev_fd = pipefd[P_READ];
	close(pipefd[P_WRITE]);
	lst_pipefd_remove_last(&data->listfd);
	data->is_exit = 0;
	data->is_pipe = TRUE;
	data->pipe_count--;
	execute_tree(top->right, data);
	if (data->prev_fd != STDIN_FILENO)
	{
		close(data->prev_fd);
		data->prev_fd = STDIN_FILENO;
	}
	data->is_pipe = FALSE;
	return (exit_status(LOAD) == SUCCESS);
}
