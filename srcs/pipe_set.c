/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_set.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 20:38:32 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/15 21:15:29 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "parse.h"
#include "util.h"

static int	count_cmd(t_token *unit);
static int	set_in_fd(t_token *unit, t_data *data);
static int	set_out_fd(t_token *unit, t_data *data);

char	**set_cmd(t_token *unit)
{
	char	**cmd;
	int		cmd_idx;
	int		count;
	t_token	*search;

	count = count_cmd(unit);
	if (count == 0)
	{
		exit_status(EXIT_SUCCESS);
		return (0);
	}
	cmd = (char **)malloc(sizeof(char *) * (count + 1));
	if (!cmd)
		exit(EXIT_FAILURE);
	cmd_idx = 0;
	search = unit;
	while (search)
	{
		cmd[cmd_idx++] = ft_strdup(search->token);
		search = search->right;
	}
	cmd[cmd_idx] = 0;
	return (cmd);
}

static int	count_cmd(t_token *unit)
{
	t_token	*search;
	int		count;

	if (unit->type == REDIRECTION)
		return (0);
	count = 0;
	search = unit;
	while (search)
	{
		count++;
		search = search->right;
	}
	return (count);
}

int	set_fd(t_token *unit, t_data *data)
{
	data->in_fd = data->prev_fd;
	if (set_in_fd(unit, data) == FAILURE)
		return (FAILURE);
	if (data->pipe_count)
	{
		_pipe(data->pipefd);
		data->out_fd = data->pipefd[P_WRITE];
	}
	else
		data->out_fd = STDOUT_FILENO;
	if (set_out_fd(unit, data) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

static int	set_in_fd(t_token *unit, t_data *data)
{
	while (unit)
	{
		if (!ft_strncmp(unit->token, "<<", 3))
		{
			if (data->in_fd != STDIN_FILENO)
				close(data->in_fd);
			data->in_fd = open_heredoc(unit);
		}
		else if (!ft_strncmp(unit->token, "<", 2))
		{
			if (data->in_fd != STDIN_FILENO)
				close(data->in_fd);
			data->in_fd = open(unit->right->token, O_RDONLY);
		}
		if (data->in_fd == FAILURE)
		{
			perror(unit->right->token);
			exit_status(256 * EXIT_FAILURE);
			return (FAILURE);
		}
		unit = unit->left;
	}
	return (SUCCESS);
}

static int	set_out_fd(t_token *unit, t_data *data)
{
	while (unit)
	{
		if (!ft_strncmp(unit->token, ">>", 3))
		{
			if (data->out_fd != STDOUT_FILENO)
				close(data->out_fd);
			data->out_fd = open(unit->right->token, O_WRONLY | O_APPEND | O_CREAT, 0644);
		}
		else if (!ft_strncmp(unit->token, ">", 2))
		{
			if (data->out_fd != STDOUT_FILENO)
				close(data->out_fd);
			data->out_fd = open(unit->right->token, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		}
		if (data->out_fd == FAILURE)
		{
			ft_putstr_fd("minishell", STDERR_FILENO);
			perror(unit->right->token);
			exit_status(256 * EXIT_FAILURE);
			return (FAILURE);
		}
		unit = unit->left;
	}
	return (SUCCESS);
}
