/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execute_tree2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 17:14:04 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/26 17:15:45 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"
#include "parse.h"

static void	wait_and_set_prev_fd(t_data *data);

void	execute_and(t_token *top, t_data *data)
{
	data->cmd_count++;
	execute_tree(top->left, data);
	if (data->pipe_count == 0)
		wait_and_set_prev_fd(data);
	data->cmd_count--;
	if (exit_status(LOAD) == EXIT_SUCCESS && data->is_exit == 0)
	{
		execute_tree(top->right, data);
		if (data->pipe_count == 0)
			wait_and_set_prev_fd(data);
	}
}

void	execute_or(t_token *top, t_data *data)
{
	data->cmd_count++;
	execute_tree(top->left, data);
	if (data->pipe_count == 0)
		wait_and_set_prev_fd(data);
	data->cmd_count--;
	if (exit_status(LOAD) != EXIT_SUCCESS && data->is_exit == 0)
	{
		execute_tree(top->right, data);
		if (data->pipe_count == 0)
			wait_and_set_prev_fd(data);
	}
}

static void	wait_and_set_prev_fd(t_data *data)
{
	if (data->last_fd != STDIN_FILENO)
		data->prev_fd = data->last_fd;
	else
		data->prev_fd = STDIN_FILENO;
	wait_all(data);
}
