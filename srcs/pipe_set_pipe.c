/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_set_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 20:38:32 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/14 16:26:29 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"

static int	is_builtin_func(t_data *data);

int	chk_cmd(t_data *data)
{
	char	**paths;
	int		idx;
	char	*tmp_path;

	if (ft_strlen(data->cmd_arr[0]) != 0)
	{
		if (is_builtin_func(data) == SUCCESS)
			return (SUCCESS);
		paths = get_paths(data);
		idx = 0;
		while (paths && paths[idx])
		{
			tmp_path = make_real_path(paths[idx], data->cmd_arr[0]);
			if (access(tmp_path, F_OK) == SUCCESS)
			{
				free(tmp_path);
				free_arr((void **)paths);
				return (SUCCESS);
			}
			idx++;
			free(tmp_path);
		}
		if (paths)
			free_arr((void **)paths);
		if (access(data->cmd_arr[0], F_OK) == SUCCESS)
			return (SUCCESS);
	}
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(data->cmd_arr[0], STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
	exit_status(256 * 127);
	return (FAILURE);
}

static int	is_builtin_func(t_data *data)
{
	if (ft_strncmp(data->cmd_arr[0], "export", 7) == 0)
		data->is_built_in = EXPORT;
	else if (ft_strncmp(data->cmd_arr[0], "env", 4) == 0)
		data->is_built_in = ENV;
	else if (ft_strncmp(data->cmd_arr[0], "unset", 6) == 0)
		data->is_built_in = UNSET;
	else if (ft_strncmp(data->cmd_arr[0], "exit", 5) == 0)
		data->is_built_in = EXIT;
	else if (ft_strncmp(data->cmd_arr[0], "cd", 3) == 0)
		data->is_built_in = CD;
	else if (ft_strncmp(data->cmd_arr[0], "pwd", 4) == 0)
		data->is_built_in = PWD;
	else if (ft_strncmp(data->cmd_arr[0], "echo", 5) == 0)
		data->is_built_in = _ECHO;
	else
		return (FAILURE);
	return (SUCCESS);
}

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

int	count_cmd(t_token *unit)
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

int	set_in_fd(t_token *unit, t_data *data)
{
	t_token	*search;

	search = unit;
	while (search)
	{
		if (!ft_strncmp(search->token, "<<", 3))
		{
			if (data->in_fd != STDIN_FILENO)
				close(data->in_fd);
			data->in_fd = open_heredoc(search);
		}
		else if (!ft_strncmp(search->token, "<", 2))
		{
			if (data->in_fd != STDIN_FILENO)
				close(data->in_fd);
			data->in_fd = open(search->right->token, O_RDONLY);
		}
		if (data->in_fd == FAILURE)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			perror(search->right->token);
			exit_status(256 * EXIT_FAILURE);
			return (FAILURE);
		}
		search = search->left;
	}
	return (SUCCESS);
}

int	set_out_fd(t_token *unit, t_data *data)
{
	t_token	*search;

	search = unit;
	while (search)
	{
		if (!ft_strncmp(search->token, ">>", 3))
		{
			if (data->out_fd != STDOUT_FILENO)
				close(data->out_fd);
			data->out_fd = open(search->right->token, O_WRONLY | O_APPEND | O_CREAT, 0644);
		}
		else if (!ft_strncmp(search->token, ">", 2))
		{
			if (data->out_fd != STDOUT_FILENO)
				close(data->out_fd);
			data->out_fd = open(search->right->token, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		}
		if (data->out_fd == FAILURE)
		{
			ft_putstr_fd("minishell", STDERR_FILENO);
			perror(search->right->token);
			exit_status(256 * EXIT_FAILURE);
			return (FAILURE);
		}
		search = search->left;
	}
	return (SUCCESS);
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
