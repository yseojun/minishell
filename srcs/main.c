/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:40:21 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/26 12:37:55 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"
#include "parse.h"

int	make_unit(t_pipe *info, int idx)
{
	int	token_start_idx;
	int	unit_idx;

	token_start_idx = idx;
	info->unit_size = 0;
	while (info->token_arr[idx])
	{
		if (is_pipe(info->token_arr[idx]))
		{
			info->unit_size++;
			break ;
		}
		idx++;
		info->unit_size++;
	}
	info->unit = (char **)malloc(sizeof(char *) * (info->unit_size + 1));
	unit_idx = 0;
	while (unit_idx < info->unit_size)
		info->unit[unit_idx++] = info->token_arr[token_start_idx++];
	info->unit[unit_idx] = 0;
	return (unit_idx);
}

int	main(void)
{
	t_pipe	pipe_info;
	t_data	data;
	char	*str;
	int		idx;
	int		unit_num;

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
		if (parse_line(str, &pipe_info) < 0)
			continue ;
		idx = 0;
		unit_num = 0;
		while (idx < pipe_info.token_arr_size)
		{
			idx += make_unit(&pipe_info, idx);
			if (run_unit(&pipe_info, &data) < 0)
				break ;
			unit_num++;
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
	free_arr((void **) info->token_arr);
}
