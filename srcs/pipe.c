/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:12:20 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/18 19:47:30 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe.h"

void	run_pipe(t_pipe *info)
{
	int		pipefd[2];
	int		prev_fd;
	pid_t	pid;

	prev_fd = info->infile_fd;
	while (info->now_idx + 1 < info->ac)
	{
		if (pipe(pipefd) < 0)
			perror_exit("pipe");
		pid = fork();
		if (pid == -1)
			perror_exit("fork");
		else if (pid == 0)
			child(pipefd, prev_fd, info);
		else
		{
			add_pid(info, pid);
			close(pipefd[1]);
			close(prev_fd);
			prev_fd = pipefd[0];
		}
		info->now_idx++;
	}
	wait_all(info);
}

void	child(int *pipefd, int prev_fd, t_pipe *info)
{
	if ('<' == )
		dup2(prev_fd, 0);
	close(prev_fd);
	if (info->now_idx + 2 != info->ac)
		dup2(pipefd[1], 1);
	else
	{
		info->outfile_fd = outfile_chk(info->av[info->ac - 1]);
		dup2(info->outfile_fd, 1);
		close(info->outfile_fd);
	}
	close(pipefd[1]);
	close(pipefd[0]);
	run_command(info);
}

void	run_command(t_pipe *info)
{
	char	**command;
	char	*path_command;

	command = ft_split(info->av[info->now_idx], ' ');
	path_command = find_command_in_path(command[0], info->path);
	if (!path_command)
	{
		ft_putstr_fd("pipex: command not found: ", 2);
		ft_putendl_fd(command[0], 2);
		exit(127);
	}
	else if (access(path_command, X_OK) == -1)
		perror_exit(command[0]);
	execve(path_command, command, info->envp);
	perror_exit(command[0]);
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
