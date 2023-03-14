/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_util.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/16 19:36:07 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/14 14:38:17 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"
#include "base.h"

void	init_pipe_info(t_pipe *info)
{
	info->prev_fd = STDIN_FILENO;
	info->wildcard = 0;
	info->cmd_arr = 0;
	info->pids = 0;
	info->pipe_count = 0;
	info->is_pipe = 0;
	info->head = 0;
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

char	*get_env(t_data *data, char *key)
{
	t_env	*search;

	search = data->env;
	while (search)
	{
		if (ft_strncmp(search->name, key, ft_strlen(key)) == 0)
			return (search->value);
		search = search->next;
	}
	return (NULL);
}

char	**get_paths(t_data *data)
{
	char	*path_value;
	char	**paths;

	path_value = get_env(data, "PATH");
	if (!path_value)
		return (NULL);
	paths = ft_split(get_env(data, "PATH"), ':');
	if (!paths)
		exit(EXIT_FAILURE);
	return (paths);
}

char	*find_command_in_path(char *command, t_data *data)
{
	char	**paths;
	int		idx;
	char	*tmp_path;

	paths = get_paths(data);
	if (!paths)
		return (command);
	idx = 0;
	while (paths[idx])
	{
		tmp_path = make_real_path(paths[idx], command);
		if (access(tmp_path, F_OK) == SUCCESS)
		{
			free_arr((void **)paths);
			return (tmp_path);
		}
		free(tmp_path);
		idx++;
	}
	free_arr((void **)paths);
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
	t_pid	*search;
	t_pid	*to_delete;
	int		status;

	search = info->pids;
	while (search)
	{
		waitpid(search->pid, &status, 0);
		to_delete = search;
		search = search->next;
		free(to_delete);
		exit_status(status);
	}
	info->pids = 0;
}
