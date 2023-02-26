/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_util.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/16 19:36:07 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/26 15:17:02 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"
#include "base.h"

void	init_pipe_info(t_pipe *info)
{
	info->unit_size = 0;
	info->prev_fd = STDIN_FILENO;
	info->token_arr = 0;
	info->cmd_arr = 0;
	info->path = ft_split(getenv("PATH"), ':');
	info->pids = 0;
}

char	*make_real_path(char *path, char *command)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*str;

	if (!path || !command)
		return (NULL);
	s1_len = ft_strlen(path);
	s2_len = ft_strlen(command);
	str = (char *)malloc(s1_len + s2_len + 2);
	if (!str)
		return (NULL);
	ft_memcpy(str, path, s1_len);
	str[s1_len] = '/';
	ft_memcpy(str + s1_len + 1, command, s2_len + 1);
	return (str);
}

char	*find_command_in_path(char *command, char **path)
{
	int		idx;
	char	*tmp_path;

	idx = 0;
	while (path[idx])
	{
		tmp_path = make_real_path(path[idx], command);
		if (access(tmp_path, F_OK) == SUCCESS)
			return (tmp_path);
		free(tmp_path);
		idx++;
	}
	return (command);
}

void	add_pid(t_pipe *info, pid_t	pid)
{
	t_pid	*last;
	t_pid	*new;

	new = (t_pid *)malloc(sizeof(t_pid));
	new->pid = pid;
	new->next = 0;
	if (info->pids)
	{
		last = info->pids;
		while (last->next)
			last = last->next;
		last->next = new;
	}
	else
		info->pids = new;
}

void	wait_all(t_pipe *info)
{
	int		status;
	t_pid	*search;

	search = info->pids;
	while (search)
	{
		waitpid(search->pid, &status, 0);
		search = search->next;
	}
}
