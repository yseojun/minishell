/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execute_tree2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 17:14:04 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/26 18:59:59 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"
#include "parse.h"

static void	wait_and_set_prev_fd(t_data *data);

void	execute_and(t_token *top, t_data *data)
{
	execute_tree(top->left, data);
	wait_and_set_prev_fd(data);
	if (exit_status(LOAD) == EXIT_SUCCESS)
	{
		execute_tree(top->right, data);
		wait_and_set_prev_fd(data);
	}
}

void	execute_or(t_token *top, t_data *data)
{
	execute_tree(top->left, data);
	wait_and_set_prev_fd(data);
	if (exit_status(LOAD) != EXIT_SUCCESS)
	{
		execute_tree(top->right, data);
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
