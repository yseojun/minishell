/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 14:25:04 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/18 17:51:51 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

int	main(void)
{
	char	*str;
	int		i;
	t_data	data;

	while (1)
	{
		i = 0;
		ft_putstr_fd("minishell> ", 1);
		str = get_next_line(0);
		if (!str) // exit, ctrl C ctrl D
			break ;
		data.tmp = ft_split(str, ' ');
		data.tmp_size = chk_tmp_size(data.tmp);
		while (i < data.tmp_size)
		{
			data.command = get_command(data.tmp);
			run_tmp(&data); // pipe exit하면 어떻게?
			i++;
		}
		free(str);
		free_arr(data.tmp);
		str = 0;
	}
}

int	run_tmp(char **tmp, t_data *data)
{
	check(tmp); //리다이렉션일 때 인파일 아웃파일 체크 //파이프 유무, 따옹표 유무, 리다이렉션 유무
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

void	free_arr(void **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}
