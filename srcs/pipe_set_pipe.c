/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_set_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 20:38:32 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/28 20:52:26 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"

static int	is_builin_func(t_pipe *info);

int	chk_cmd(t_pipe *info, t_data *data)
{
	char	**paths;
	int		idx;
	char	*tmp_path;

	paths = get_paths(data);
	idx = 0;
	info->is_built_in = 0;
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
	if (is_builin_func(info) == SUCCESS)
		return (SUCCESS);
	ft_putstr_fd(CMD_NOT_FOUND, STDERR_FILENO); // sig?
	ft_putendl_fd(info->cmd_arr[0], STDERR_FILENO);
	return (FAILURE);
}

static int	is_builin_func(t_pipe *info)
{
	if (ft_strncmp(info->cmd_arr[0], "export", 7) == 0)
		info->is_built_in = EXPORT;
	else if (ft_strncmp(info->cmd_arr[0], "env", 4) == 0)
		info->is_built_in = ENV;
	else if (ft_strncmp(info->cmd_arr[0], "unset", 6) == 0)
		info->is_built_in = UNSET;
	else if (ft_strncmp(info->cmd_arr[0], "exit", 5) == 0)
		info->is_built_in = EXIT;
	// else if (ft_strncmp(info->cmd_arr[0], "cd", 2) == 0)
	// 	info->is_built_in = CD;
	// else if (ft_strncmp(info->cmd_arr[0], "pwd", 3) == 0)
	// 	info->is_built_in = PWD;
	// else if (ft_strncmp(info->cmd_arr[0], "echo", 4) == 0)
	// 	info->is_built_in = ECHO;
	// else if (ft_strncmp(info->cmd_arr[0], "history", 7) == 0)
	// 	info->is_built_in = HISTORY;
	else
		return (FAILURE);
	return (SUCCESS);
}

char	**set_cmd(char **tmp)
{
	char	**cmd;
	int		idx;
	int		cmd_idx;
	int		count;

	count = count_cmd(tmp);
	if (count == 0)
		return (0);
	cmd = (char **)malloc(sizeof(char *) * (count + 1));
	if (!cmd)
		exit(EXIT_FAILURE);
	idx = 0;
	cmd_idx = 0;
	while (tmp[idx])
	{
		if (is_redirection(tmp[idx]))
			idx++;
		else if (!is_pipe(tmp[idx]))
			cmd[cmd_idx++] = ft_strdup(tmp[idx]);
		idx++;
	}
	cmd[cmd_idx] = 0;
	return (cmd);
}

int	count_cmd(char **tmp)
{
	int	idx;
	int	count;

	idx = 0;
	count = 0;
	while (tmp[idx])
	{
		if (is_redirection(tmp[idx]))
			idx++;
		else if (!is_pipe(tmp[idx]))
			count++;
		idx++;
	}
	return (count);
}

static int	set_infile(t_pipe *info, char **unit)
{
	int	idx;

	idx = 0;
	while (unit[idx])
	{
		if (!ft_strncmp(unit[idx], "<<", 3))
		{
			if (info->infile_fd != STDIN_FILENO)
				close(info->infile_fd);
			info->infile_fd = make_heredoc(unit[idx + 1]);
		}
		else if (!ft_strncmp(unit[idx], "<", 2))
		{
			if (info->infile_fd != STDIN_FILENO)
				close(info->infile_fd);
			info->infile_fd = infile_chk(unit[idx + 1]);
		}
		if (info->infile_fd == FAILURE)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			perror(unit[idx + 1]);
			return (FAILURE);
		}
		idx++;
	}
	return (SUCCESS);
}

static int	set_outfile(t_pipe *info, char **unit)
{
	int	idx;

	idx = 0;
	while (unit[idx])
	{
		if (!ft_strncmp(unit[idx], ">>", 3))
		{
			if (info->outfile_fd != STDOUT_FILENO)
				close(info->outfile_fd);
			info->outfile_fd = \
			open(unit[idx + 1], O_WRONLY | O_APPEND | O_CREAT, 0644);
		}
		else if (!ft_strncmp(unit[idx], ">", 2))
		{
			if (info->outfile_fd != STDOUT_FILENO)
				close(info->outfile_fd);
			info->outfile_fd = \
			open(unit[idx + 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
		}
		if (info->outfile_fd == FAILURE)
		{
			ft_putstr_fd("minishell", STDERR_FILENO);
			perror(unit[idx + 1]);
			return (FAILURE);
		}
		idx++;
	}
	return (SUCCESS);
}

int	set_fd(t_pipe *info)
{
	info->infile_fd = STDIN_FILENO;
	info->outfile_fd = STDOUT_FILENO;
	if (set_infile(info, info->unit) < 0 || set_outfile(info, info->unit) < 0)
		return (FAILURE);
	info->in_fd = info->prev_fd;
	if (info->infile_fd != STDIN_FILENO)
	{
		if (info->prev_fd != STDIN_FILENO)
			close(info->prev_fd);
		info->in_fd = info->infile_fd;
	}
	printf("last unit : %s\n", info->unit[info->unit_size - 1]);
	if (is_pipe(info->unit[info->unit_size - 1]))
	{
		_pipe(info->pipefd);
		info->is_pipe = 1;
		info->out_fd = info->pipefd[1];
	}
	else
		info->out_fd = STDOUT_FILENO;
	if (info->outfile_fd != STDOUT_FILENO)
	{
		if (info->is_pipe)
			close(info->pipefd[1]);
		info->out_fd = info->outfile_fd;
	}
	return (SUCCESS);
}
