/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:40:21 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/18 20:25:50 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "pipe.h"

int	main(void)
{
	char	*str;
	int		i;
	t_data	data;
	t_pipe	pipe_info;
	char	*tmp2;

	init_info(&pipe_info, environ);
	while (1)
	{
		i = 0;
		ft_putstr_fd("minishell> ", 1);
		str = get_next_line(0);
		if (!str) // exit, ctrl C ctrl D
			break ;
		data.tmp = ft_split(str, '|');
		data.tmp_size = chk_tmp_size(data.tmp);

		int		pipefd[2];
		int		prev_fd;
		pid_t	pid;
		while (i < data.tmp_size)
		{
			// << < 가 들어왔다면 : 다음 파일이 존재하는지 확인하고 stdin으로 연결 (인덱스 두 번 넘기기)
			// >> > 가 들어왔다면 : 다음 파일을 stdout으로 연결 (인덱스 두 번 넘기기)
			// | 가 들어왔다면 : 파이프 READ_END를 stdout에 연결
			// command가 들어왔다면 : 이후의 인덱스를 보면서 stdin, stdout 연결
				// > 또는 >> 또는 더 이상 없을 때까지 while문 돌면서 확인
					// < 가 있다면 그 다음 파일이 존재하는지 확인하고 stdin으로 연결
					// > 또는 >> 가 있다면 그 다음 파일을 stdout으로 연결
					// | 가 있다면 파이프 READ_END를 stdout으로 연결
					// 더이상 없다면 stdout을 stdout으로 연결
				// 실행하기⭐️
			
			tmp2 = ft_split(str, ' ');
			set_infile(tmp2); // heredoc 처리
			set_outfile(tmp2);
			set_command(tmp2);
			prev_fd = info->infile_fd;
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
			i++;
		}
		wait_all(info);
		free(str);
		free_arr(data.tmp);
		str = 0;
	}
}

void	free_arr(void **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}
