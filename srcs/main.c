/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojun <seojun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:40:21 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/08 15:39:36 by seojun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"
#include "parse.h"

static void	manage_signals(void);
static void	handler(int sig);
static void	finish_line(char *str, t_pipe *info);

int	main(void)
{
	t_pipe	line_info;
	t_data	data;
	char	*str;

	manage_signals();
	init_data(&data);
	while (1)
	{
		line_info.heredoc_tmp = 0;
		printf("%s", ft_itoa((int) getpid()));
		init_pipe_info(&line_info);
		str = readline("minishell> ");
		if (!str)
			break ;
		add_history(str);
		if (parse_line(str, &data, &line_info) == FAILURE)
			continue ;
		excute_tree(line_info.head, &line_info, &data);
		finish_line(str, &line_info);
	}
	return (SUCCESS);
}

static void	manage_signals(void)
{
	extern int	rl_catch_signals;

	rl_catch_signals = 0;
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
}

static void	handler(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

static void	finish_line(char *str, t_pipe *info)
{
	if (info->heredoc_tmp)
	{
		unlink(info->heredoc_tmp);
		free(info->heredoc_tmp);
		info->heredoc_tmp = 0;
	}
	free(str);
}
