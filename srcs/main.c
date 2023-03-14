/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:40:21 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/14 14:37:09 by rolee            ###   ########.fr       */
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
		exit_status(0);
		find_heredoc(line_info.head);
		if (exit_status(LOAD) == EXIT_SUCCESS)
			excute_tree(line_info.head, &line_info, &data);
		finish_line(str, &line_info);
	}
	// system("leaks --quiet minishell");
	return (SUCCESS);
}

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
		exit_status(256 * EXIT_FAILURE);
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
		ft_putendl_fd("Quit: 3", STDERR_FILENO);
}

int	exit_status(int status)
{
	static int	exit_status;

	if (status == LOAD)
		return (exit_status);
	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		exit_status = 128 + WTERMSIG(status);
	//printf("status: %d, exit_status: %d\n", status, exit_status);
	return (exit_status);
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
	unlink_heredoc(info->head);
	lst_tree_free_all(info->head);
	info->head = 0;
	// set_status(-2);
	// system("leaks --quiet minishell");
}
