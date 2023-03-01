/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:40:21 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/01 16:21:30 by seojyang         ###   ########.fr       */
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
	t_pipe	line_info;
	t_data	data;
	char	*str;
	int		idx;

	init_data(&data);
	signal(SIGINT, handler);
	signal(SIGQUIT, handler); // ctrl + \ 눌렀을 때
	while (1)
	{
		init_pipe_info(&line_info);
		str = readline("minishell> ");
		if (!str)
			break ;
		add_history(str);
		if (parse_line(str, &data, &line_info) == FAILURE)
			continue ;
		idx = 0;
		prt_arr(line_info.token_arr);
		printf("\n");
		while (idx < line_info.token_arr_size)
		{
			idx += make_unit(&line_info, idx);
			prt_arr(line_info.unit);
			if (run_unit(&line_info, &data) == FAILURE)
				continue ;
		}
		finish_line(str, &line_info, &data);
	}
	return (SUCCESS);
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

void	finish_line(char *str, t_pipe *info, t_data *data)
{
	if (info->is_pipe)
		close(info->pipefd[P_READ]);
	wait_all(info, data);
	free(str);
	free_arr((void **) info->token_arr);
}
