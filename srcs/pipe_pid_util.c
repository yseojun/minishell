/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_pid_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 16:11:44 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/14 16:11:55 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

void	add_pid(t_data *data, pid_t	pid)
{
	t_pid	*last;
	t_pid	*new;

	new = (t_pid *)malloc(sizeof(t_pid));
	new->pid = pid;
	new->next = 0;
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

	search = data->pids;
	while (search)
	{
		waitpid(search->pid, &status, 0);
		to_delete = search;
		search = search->next;
		free(to_delete);
		exit_status(status);
	}
	data->pids = 0;
}
