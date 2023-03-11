/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_set_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 20:38:32 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/11 20:32:33 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"

static int	is_builtin_func(t_pipe *info);

int	chk_cmd(t_pipe *info, t_data *data)
{
	char	**paths;
	int		idx;
	char	*tmp_path;

	if (ft_strlen(info->cmd_arr[0]) != 0)
	{
		paths = get_paths(data);
		idx = 0;
		if (is_builtin_func(info) == SUCCESS)
			return (SUCCESS);
		while (paths && paths[idx])
		{
			tmp_path = make_real_path(paths[idx], info->cmd_arr[0]);
			if (access(tmp_path, F_OK) == SUCCESS)
			{
				free(tmp_path);
				return (SUCCESS);
			}
			idx++;
			free(tmp_path);
		}
		if (paths)
			free_arr((void **)paths);
		if (access(info->cmd_arr[0], F_OK) == SUCCESS)
			return (SUCCESS);
	}
	data->exit_status = 127;
	ft_putstr_fd(CMD_NOT_FOUND, STDERR_FILENO);
	ft_putendl_fd(info->cmd_arr[0], STDERR_FILENO);
	return (FAILURE);
}

static int	is_builtin_func(t_pipe *info)
{
	if (ft_strncmp(info->cmd_arr[0], "export", 7) == 0)
		info->is_built_in = EXPORT;
	else if (ft_strncmp(info->cmd_arr[0], "env", 4) == 0)
		info->is_built_in = ENV;
	else if (ft_strncmp(info->cmd_arr[0], "unset", 6) == 0)
		info->is_built_in = UNSET;
	else if (ft_strncmp(info->cmd_arr[0], "exit", 5) == 0)
		info->is_built_in = EXIT;
	else if (ft_strncmp(info->cmd_arr[0], "cd", 3) == 0)
		info->is_built_in = CD;
	else if (ft_strncmp(info->cmd_arr[0], "pwd", 4) == 0)
		info->is_built_in = PWD;
	else if (ft_strncmp(info->cmd_arr[0], "echo", 5) == 0)
		info->is_built_in = _ECHO;
	// else if (ft_strncmp(info->cmd_arr[0], "history", 8) == 0)
	// 	info->is_built_in = HISTORY;
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
		return (0);
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

int	set_in_fd(t_token *unit, t_pipe *info)
{
	t_token	*search;

	search = unit;
	while (search)
	{
		if (!ft_strncmp(search->token, "<<", 3))
		{
			if (info->in_fd != STDIN_FILENO)
				close(info->in_fd);
			info->in_fd = make_heredoc(search->right->token, info);
		}
		else if (!ft_strncmp(search->token, "<", 2))
		{
			if (info->in_fd != STDIN_FILENO)
				close(info->in_fd);
			info->in_fd = infile_chk(search->right->token);
		}
		if (info->in_fd == FAILURE)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			perror(search->right->token);
			return (FAILURE);
		}
		search = search->left;
	}
	return (SUCCESS);
}

int	set_out_fd(t_token *unit, t_pipe *info)
{
	t_token	*search;

	search = unit;
	while (search)
	{
		if (!ft_strncmp(search->token, ">>", 3))
		{
			if (info->out_fd != STDOUT_FILENO)
				close(info->out_fd);
			info->out_fd = open(search->right->token, O_WRONLY | O_APPEND | O_CREAT, 0644);
		}
		else if (!ft_strncmp(search->token, ">", 2))
		{
			if (info->out_fd != STDOUT_FILENO)
				close(info->out_fd);
			info->out_fd = open(search->right->token, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		}
		if (info->out_fd == FAILURE)
		{
			ft_putstr_fd("minishell", STDERR_FILENO);
			perror(search->right->token);
			return (FAILURE);
		}
		search = search->left;
	}
	return (SUCCESS);
}

int	set_fd(t_token *unit, t_pipe *info)
{
	// 리다이렉션 연결하기
	// in_fd
	info->in_fd = info->prev_fd; // pipe 또는 STDIN으로 기본 세팅
	if (set_in_fd(unit, info) == FAILURE) // unit left를 보며 재세팅
		return (FAILURE);
	// out_fd
	if (info->pipe_count) // pipe 또는 STDOUT으로 기본 세팅
	{
		_pipe(info->pipefd);
		info->out_fd = info->pipefd[P_WRITE];
	}
	else
		info->out_fd = STDOUT_FILENO;
	if (set_out_fd(unit, info) == FAILURE) // unit left를 보며 재세팅
		return (FAILURE);
	return (SUCCESS);
}
