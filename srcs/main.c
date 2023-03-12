/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:40:21 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/12 19:50:22 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"
#include "parse.h"

static void	manage_signals(void);
void	handler(int sig);
static void	finish_line(char *str, t_pipe *info);

int	main(int argc, char *argv[], char *envp[])
{
	t_pipe	line_info;
	t_data	data;
	char	*str;

	argv = 0;
	if (argc != 1)
	{
		ft_putendl_fd("Invalid arguments", STDERR_FILENO);
		return (FAILURE);
	}
	manage_signals();
	init_data(&data, envp);
	while (1)
	{
		line_info.heredoc_tmp = 0;
		//printf("%s", ft_itoa((int) getpid()));
		init_pipe_info(&line_info);
		str = readline("minishell> ");
		if (!str)
			break ;
		add_history(str);
		if (parse_line(str, &data, &line_info) == FAILURE)
		{
			free(str);
			continue ;
		}
		excute_tree(line_info.head, &line_info, &data);
		//load_status(&data);
		finish_line(str, &line_info);
	}
	// system("leaks --quiet minishell");
	return (SUCCESS);
}

void	child_handler(int sig);

static void	manage_signals(void)
{
	extern int	rl_catch_signals;

	rl_catch_signals = 0;
	old = signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
}

void	handler(int sig)
{
	if (sig == SIGINT)
	{
		set_status(256);
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	set_status(int status)
{
	static int	save;

	if (status == -1)
		return (save);
	// if (WIFSIGNALED(status))
	// {
	// 	printf("시그널드, %d\n", WTERMSIG(status));
	// 	save = 128 + WTERMSIG(status);
	// }
	// else if (WIFEXITED(status))
	// {
	// 	printf("엑시티드\n");
	// 	save = status;
	// }
	if (WIFEXITED(status))
	{
		printf("엑시티드\n");
		save = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status))
	{
		printf("시그널드, %d\n", WTERMSIG(status));
		save = 128 + WTERMSIG(status);
	}
	return (save);
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
	free_arr((void **)info->cmd_arr);
	lst_tree_free_all(info->head);
	info->head = 0;
	// set_status(-2);
	// system("leaks --quiet minishell");
}
