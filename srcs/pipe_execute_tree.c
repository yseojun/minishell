/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execute_tree.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 16:25:31 by rolee             #+#    #+#             */
/*   Updated: 2023/03/15 20:48:26 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"

static int	execute_and(t_token *top, t_data *data);
static int	execute_or(t_token *top, t_data *data);
static int	execute_pipe(t_token *top, t_data *data);

int	excute_tree(t_token *top, t_data *data)
{
	if (top == 0)
		return (1);
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
		run_unit(top, data);
		return (exit_status(LOAD) == SUCCESS);
	}
}

static int	execute_and(t_token *top, t_data *data)
{
	if (excute_tree(top->left, data) == TRUE)
	{
		data->prev_fd = STDIN_FILENO;
		return (excute_tree(top->right, data));
	}
	return (TRUE);
}

static int	execute_or(t_token *top, t_data *data)
{
	if (excute_tree(top->left, data) == FALSE)
	{
		data->prev_fd = STDIN_FILENO;
		return (excute_tree(top->right, data));
	}
	return (TRUE);
}

static int	execute_pipe(t_token *top, t_data *data)
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
