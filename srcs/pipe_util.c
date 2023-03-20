/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_util.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/16 19:36:07 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/20 20:54:24 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"

void	reset_line_data(t_data *data)
{
	data->prev_fd = STDIN_FILENO;
	data->wildcard = 0;
	data->cmd_arr = 0;
	data->pids = 0;
	data->listfd = 0;
	data->pipe_count = 0;
	data->is_pipe = 0;
	data->head = 0;
	data->is_exit = 0;
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

void	add_pid(t_data *data, pid_t	pid)
{
	t_pid	*last;
	t_pid	*new;

	new = (t_pid *)malloc(sizeof(t_pid));
	if (!new)
		exit(EXIT_FAILURE);
	new->pid = pid;
	new->next = 0;
	// printf("newpid: %d\n", new->pid);
	if (data->pids)
	{
		last = data->pids;
		while (last->next)
			last = last->next;
		last->next = new;
	}
	else
		data->pids = new;
}

void	wait_all(t_data *data)
{
	t_pid	*search;
	t_pid	*to_delete;
	int		status;

	// search = data->pids;
	// while (search)
	// {
	// 	search = search->next;
	// }
	search = data->pids;
	while (search)
	{
		//printf("2\n");
		waitpid(search->pid, &status, 0);
		printf("%d\n", search->pid);
		//printf("3\n");
		to_delete = search;
		search = search->next;
		free(to_delete);
		exit_status(status);
	}
	printf("fin\n");
	data->pids = 0;
}
