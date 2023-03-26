/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execute_tree.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 16:25:31 by rolee             #+#    #+#             */
/*   Updated: 2023/03/26 19:08:44 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"
#include "parse.h"

static void	execute_pipe(t_token *top, t_data *data);
static void	execute_brace(t_token *top, t_data *data);

void	execute_tree(t_token *top, t_data *data)
{
	if (top == NULL)
		return ;
	if (top->type == BRACE)
		execute_brace(top, data);
	else if (top->type == AND)
		execute_and(top, data);
	else if (top->type == OR)
		execute_or(top, data);
	else if (top->type == PIPE)
		execute_pipe(top, data);
	else
	{
		free_arr((void **)data->cmd_arr);
		data->cmd_arr = 0;
		transform(data, top);
		run_unit(top, data);
	}
}

static void	execute_brace(t_token *top, t_data *data)
{
	pid_t	pid;

	pid = _fork();
	if (pid == 0)
	{
		data->pids = 0;
		data->pipe_count = 0;
		data->is_pipe = 0;
		data->last_fd = data->prev_fd;
		if (data->listfd)
			close(lst_pipefd_last(data->listfd)->pipefd[P_READ]);
		execute_tree(top->left, data);
		close(data->last_fd);
		wait_all(data);
		exit(exit_status(LOAD));
	}
	add_pid(data, pid);
}

static void	execute_pipe(t_token *top, t_data *data)
{
	int	pipefd[2];

	_pipe(pipefd);
	lst_pipefd_add_back(&data->listfd, lst_new_pipefd(pipefd));
	data->is_pipe = TRUE;
	data->pipe_count++;
	execute_tree(top->left, data);
	data->prev_fd = pipefd[P_READ];
	close(pipefd[P_WRITE]);
	lst_pipefd_remove_last(&data->listfd);
	data->is_pipe = TRUE;
	data->pipe_count--;
	execute_tree(top->right, data);
	if (data->prev_fd != STDIN_FILENO)
	{
		close(data->prev_fd);
		data->prev_fd = STDIN_FILENO;
	}
	data->is_pipe = FALSE;
}
