/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_set.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 20:38:32 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/28 11:20:05 by rolee            ###   ########.fr       */
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
		if (search->type != ERROR)
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
		if (search->type != ERROR)
			count++;
		search = search->right;
	}
	return (count);
}

int	set_fd(t_token *unit, t_data *data)
{
	data->in_fd = data->prev_fd;
	if (data->listfd)
		data->out_fd = lst_pipefd_last(data->listfd)->pipefd[P_WRITE];
	else
		data->out_fd = STDOUT_FILENO;
	while (unit)
	{
		if (set_in_fd(unit, data) == FAILURE
			|| set_out_fd(unit, data) == FAILURE)
			return (FAILURE);
		if (data->in_fd == FAILURE || data->out_fd == FAILURE)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			perror(unit->right->token);
			exit_status(256 * EXIT_FAILURE);
			return (FAILURE);
		}
		unit = unit->left;
	}
	return (SUCCESS);
}

static int	set_in_fd(t_token *unit, t_data *data)
{
	if (!ft_strncmp(unit->token, "<<", 3)
		|| !ft_strncmp(unit->token, "<", 2))
	{
		if (!ft_strncmp(unit->token, "<", 2) && unit->right
			&& (unit->right->right || unit->right->type == ERROR))
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putendl_fd("ambiguous redirect", STDERR_FILENO);
			exit_status(256 * EXIT_FAILURE);
			return (FAILURE);
		}
		if (data->in_fd != STDIN_FILENO && data->in_fd != data->prev_fd)
			close(data->in_fd);
		if (!ft_strncmp(unit->token, "<<", 3))
			data->in_fd = open_heredoc(unit);
		else
			data->in_fd = open(unit->right->token, O_RDONLY);
	}
	return (SUCCESS);
}

static int	set_out_fd(t_token *unit, t_data *data)
{
	const int	opt = O_WRONLY | O_CREAT;

	if (!ft_strncmp(unit->token, ">>", 3)
		|| !ft_strncmp(unit->token, ">", 2))
	{
		if (unit->right && (unit->right->right || unit->right->type == ERROR))
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putendl_fd("ambiguous redirect", STDERR_FILENO);
			exit_status(256 * EXIT_FAILURE);
			return (FAILURE);
		}
		if (data->out_fd != STDOUT_FILENO
			&& (!data->listfd || data->out_fd != \
			lst_pipefd_last(data->listfd)->pipefd[P_WRITE]))
			close(data->out_fd);
		if (!ft_strncmp(unit->token, ">>", 3))
			data->out_fd = open(unit->right->token, opt | O_APPEND, 0644);
		else
			data->out_fd = open(unit->right->token, opt | O_TRUNC, 0644);
	}
	return (SUCCESS);
}
