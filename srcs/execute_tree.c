/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojun <seojun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 16:25:31 by rolee             #+#    #+#             */
/*   Updated: 2023/03/29 13:57:25 by seojun           ###   ########.fr       */
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
		data->is_subshell = 1;
		data->pids = 0;
		data->is_pipe = 0;
		data->last_fd = data->prev_fd;
		if (data->listfd
			&& data->prev_fd != lst_pipefd_last(data->listfd)->pipefd[P_READ])
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
	execute_tree(top->left, data);
	data->prev_fd = pipefd[P_READ];
	close(pipefd[P_WRITE]);
	lst_pipefd_remove_last(&data->listfd);
	data->is_pipe = TRUE;
	execute_tree(top->right, data);
	close(data->prev_fd);
	data->prev_fd = STDIN_FILENO;
	data->is_pipe = FALSE;
}
