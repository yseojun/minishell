/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:40:21 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/05 18:20:45 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"
#include "parse.h"

int	main(void)
{
	t_pipe	line_info;
	t_data	data;
	char	*str;
	int		idx;

	init_data(&data);
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN); // ctrl + \ 눌렀을 때
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
		excute_tree(line_info.head, &line_info, &data);
		finish_line(str, &line_info);
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

void	finish_line(char *str, t_pipe *info)
{
	if (info->is_pipe)
		close(info->pipefd[P_READ]);
	free(str);
}
