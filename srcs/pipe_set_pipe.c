/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_set_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 20:38:32 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/26 20:19:11 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

int	is_user_func(t_pipe *info);

int	chk_cmd(t_pipe *info)
{
	int		idx;
	char	*tmp_path;

	idx = 0;
	while (info->path[idx])
	{
		tmp_path = make_real_path(info->path[idx], info->cmd_arr[0]);
		if (access(tmp_path, F_OK) == SUCCESS)
		{
			free(tmp_path);
			return (SUCCESS);
		}
		idx++;
		free(tmp_path);
	}
	if (access(info->cmd_arr[0], F_OK) == SUCCESS)
		return (SUCCESS);
	if (is_user_func(info) == SUCCESS)
		return (SUCCESS);
	ft_putstr_fd(CMD_NOT_FOUND, STDERR_FILENO); // sig?
	ft_putendl_fd(info->cmd_arr[0], STDERR_FILENO);
	return (FAILURE);
}

int	is_user_func(t_pipe *info)
{
	if (ft_strncmp(info->cmd_arr[0], "export", 6) == 0)
		info->is_built_in = 1;
	else if (ft_strncmp(info->cmd_arr[0], "unset", 5) == 0)
		info->is_built_in = 2;
	else if (ft_strncmp(info->cmd_arr[0], "exit", 4) == 0)
		info->is_built_in = 3;
	else
		return (FAILURE);
	return (SUCCESS);
}

char	**set_cmd(t_pipe *info)
{
	char	**cmd;
	int		idx;
	int		cmd_idx;
	int		count;

	count = count_cmd(info);
	printf("%d\n", count);
	if (count == 0)
		return (0);
	cmd = (char **)malloc(sizeof(char *) * (count + 1));
	if (!cmd)
		exit(EXIT_FAILURE);
	idx = 0;
	cmd_idx = 0;
	while (idx < info->unit_size)
	{
		if (info->unit[idx] && !is_symbol(info->unit[idx]))
			cmd[cmd_idx++] = ft_strdup(info->unit[idx]);
		idx++;
	}
	cmd[cmd_idx] = 0;
	return (cmd);
}

int	count_cmd(t_pipe *info)
{
	int	idx;
	int	count;

	idx = 0;
	count = 0;
	while (idx < info->unit_size)
	{
		if (info->unit[idx] && !is_symbol(info->unit[idx]))
			count++;
		idx++;
	}
	return (count);
}

static int	set_in_fd(t_pipe *info, char **unit)
{
	int	idx;

	info->in_fd = info->prev_fd;
	idx = 0;
	while (idx < info->unit_size)
	{
		if (info->unit[idx] && !ft_strncmp(unit[idx], "<<", 3))
			info->in_fd = make_heredoc(unit[idx + 1]);
		else if (info->unit[idx] && !ft_strncmp(unit[idx], "<", 2))
			info->in_fd = infile_chk(unit[idx + 1]);
		if (info->in_fd == FAILURE)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			perror(unit[idx + 1]);
			return (FAILURE);
		}
		idx++;
	}
	return (SUCCESS);
}

static int	set_out_fd(t_pipe *info, char **unit)
{
	int	idx;

	if (info->unit[info->unit_size - 1] && is_pipe(unit[info->unit_size - 1]))
		info->out_fd = info->pipefd[P_WRITE];
	else
		info->out_fd = STDOUT_FILENO;
	idx = 0;
	while (idx < info->unit_size)
	{
		if (info->unit[idx] && !ft_strncmp(unit[idx], ">>", 3))
			info->out_fd = \
			open(unit[idx + 1], O_WRONLY | O_APPEND | O_CREAT, 0644);
		else if (info->unit[idx] && !ft_strncmp(unit[idx], ">", 2))
			info->out_fd = \
			open(unit[idx + 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (info->out_fd == FAILURE)
		{
			ft_putstr_fd("minishell", STDERR_FILENO);
			perror(unit[idx + 1]);
			return (FAILURE);
		}
		idx++;
	}
	return (SUCCESS);
}

// set_in_fd와 set_out_fd가 FAILURE를 뱉었을 때 처리!!
int	set_fd(t_pipe *info)
{
	if (set_in_fd(info, info->unit) == FAILURE)
		return (FAILURE);
	if (info->in_fd != info->prev_fd)
		close(info->prev_fd);
	if (set_out_fd(info, info->unit) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}
