/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_set_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 20:38:32 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/26 12:48:37 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include <fcntl.h>
#include "base.h"

// int	set_pipe(t_pipe *info, char **tmp)
// {
// 	int	idx;
// 	int	chk;

// 	idx = 0;
// 	chk = 0;
// 	info->cmd_arr = set_cmd(tmp);
// 	while (tmp[idx])
// 	{
// 		if (is_redirection(tmp[idx]))
// 		{
// 			if (set_in_out(info, tmp, idx) < 0)
// 				return (-1);
// 			idx++;
// 		}
// 		else if (chk == 0)
// 		{
// 			if (chk_cmd(info) < 0) //순서
// 				return (-1);
// 			chk = 1;
// 		}
// 		idx++;
// 	}
// 	return (0);
// }

int	chk_cmd(t_pipe *info)
{
	int		idx;
	char	*tmp_path;

	idx = 0;
	while (info->path[idx])
	{
		tmp_path = make_real_path(info->path[idx], info->cmd_arr[0]);
		if (access(tmp_path, F_OK) == 0)
		{
			free(tmp_path);
			return (0);
		}
		idx++;
		free(tmp_path);
	}
	if (access(info->cmd_arr[0], F_OK) == 0)
		return (0);
	ft_putstr_fd("minishell: command not found: ", 2); // sig?
	ft_putendl_fd(info->cmd_arr[0], 2);
	return (-1);
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
		exit(1);
	idx = 0;
	cmd_idx = 0;
	while (tmp[idx])
	{
		if (!is_symbol(tmp[idx]))
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
		if (!is_symbol(tmp[idx]))
			count++;
		idx++;
	}
	return (count);
}

// 세팅된 in_fd와 prev_fd가 다르다면 close(prev_fd);
int	set_in_fd(t_pipe *info, char **unit)
{
	int	idx;

	info->in_fd = info->prev_fd;
	idx = 0;
	while (unit[idx])
	{
		if (!ft_strncmp(unit[idx], "<<", 3))
			info->in_fd = make_heredoc(unit[idx + 1]);
		else if (!ft_strncmp(unit[idx], "<", 2))
			info->in_fd = infile_chk(unit[idx + 1]);
		if (info->in_fd < 0)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			perror(unit[idx + 1]);
			return (-1);
		}
		idx++;
	}
	return (0);
}

// unit의 마지막이 파이프가 아니면 아예 pipe 함수를 사용하지 말자!
int	set_out_fd(t_pipe *info, char **unit)
{
	int	idx;

	// if (unit의 마지막이 파이프라면)
	if (info->unit_size) //임시 처리
		info->out_fd = info->pipefd[1];
	else
		info->out_fd = STDOUT_FILENO;
	idx = 0;
	while (unit[idx])
	{
		if (!ft_strncmp(unit[idx], ">>", 3))
			info->out_fd = \
			open(unit[idx + 1], O_WRONLY | O_APPEND | O_CREAT, 0644);
		else if (!ft_strncmp(unit[idx], ">", 2))
			info->out_fd = \
			open(unit[idx + 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (info->out_fd < 0)
		{
			ft_putstr_fd("minishell", STDERR_FILENO);
			perror(unit[idx + 1]);
			return (-1);
		}
		idx++;
	}
	return (0);
}

// int	set_in_out(t_pipe *info, char **tmp, int idx) // in_out 분리
// {
// 	if (!ft_strncmp(tmp[idx], "<<", 3))
// 		info->infile_fd = make_heredoc(tmp[idx + 1]);
// 	else if (!ft_strncmp(tmp[idx], "<", 2))
// 		info->infile_fd = infile_chk(tmp[idx + 1]);
// 	else if (!ft_strncmp(tmp[idx], ">>", 3))
// 		info->outfile_fd = \
// 		open(tmp[idx + 1], O_WRONLY | O_APPEND | O_CREAT, 0644);
// 	else if (!ft_strncmp(tmp[idx], ">", 2))
// 		info->outfile_fd = \
// 		open(tmp[idx + 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
// 	if (info->infile_fd < 0 || info->outfile_fd < 0)
// 	{
// 		ft_putstr_fd("minishell: ", 2);
// 		perror(tmp[idx + 1]);
// 		return (-1);
// 	}
// 	return (0);
// }
