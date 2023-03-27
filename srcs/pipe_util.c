/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_util.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/16 19:36:07 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/27 21:37:52 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"

void	reset_line_data(t_data *data)
{
	data->prev_fd = STDIN_FILENO;
	data->wildcard = 0;
	data->listfd = 0;
	data->cmd_arr = 0;
	data->pids = 0;
	data->is_pipe = 0;
	data->head = 0;
	data->last_fd = 0;
	data->is_subshell = 0;
}

int	run_single_builtin(t_data *data)
{
	if (data->is_pipe || data->is_built_in == FALSE)
		return (FALSE);
	if (data->is_built_in == EXPORT && data->cmd_arr[1])
		exit_status(builtin_export(data, data->cmd_arr) * 256);
	else if (data->is_built_in == UNSET)
		exit_status(builtin_unset(data, data->cmd_arr) * 256);
	else if (data->is_built_in == EXIT)
		exit_status(builtin_exit(data->cmd_arr) * 256);
	else if (data->is_built_in == CD)
		exit_status(builtin_cd(data, data->cmd_arr[1]) * 256);
	else
		return (FALSE);
	return (TRUE);
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

int	chk_stat(char *path_command)
{
	struct stat	sp;

	lstat(path_command, &sp);
	if (S_ISDIR(sp.st_mode))
		return (FAILURE);
	return (SUCCESS);
}
