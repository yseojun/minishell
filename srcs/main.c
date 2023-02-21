/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:40:21 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/21 22:04:27 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"
#include "parse.h"

int	main(void)
{
	t_pipe	pipe_info;
	t_data	data;
	char	*str;
	int		i;

	init_data(&data);
	signal(SIGINT, handler);
	while (1)
	{
		// ↓ export로 path를 바꾸면 어떻게 되는거지?ㅋㅋㅋ
		init_pipe_info(&pipe_info);
		str = readline("minishell> ");
		if (!str)
			break ;
		add_history(str);
		// if (error_check(str) < 0) // 에러 체크 (||, ;, >, < 중복)
		// 	continue ;
		parse_line(str, &pipe_info);
		i = 0;
		while (i < pipe_info.tmp_size)
		{
			if (run_pipe(&pipe_info, &data, i++) < 0)
				break ;
		}
		finish_line(str, &pipe_info);
	}
}

// handler C문자 없애기
void	handler(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	finish_line(char *str, t_pipe *info)
{
	wait_all(info);
	free(str);
	free(info->tmp);
}
