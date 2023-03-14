/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:40:21 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/14 15:30:44 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"
#include "parse.h"

static void	manage_signals(void);
void		handler(int sig);
static void	finish_line(char *str, t_data *data);

int	main(int argc, char *argv[], char *envp[])
{
	t_data	data;
	char	*str;

	argv = 0;
	if (argc != 1)
	{
		ft_putendl_fd("Invalid arguments", STDERR_FILENO);
		return (FAILURE);
	}
	manage_signals();
	init_data_env(&data, envp);
	while (1)
	{
		//printf("%s", ft_itoa((int) getpid()));
		reset_line_data(&data);
		str = readline("minishell> ");
		if (!str)
			break ;
		add_history(str);
		if (parse_line(str, &data) == FAILURE)
		{
			free(str);
			continue ;
		}
		exit_status(0);
		find_heredoc(data.head);
		if (exit_status(LOAD) == EXIT_SUCCESS)
			excute_tree(data.head, &data);
		finish_line(str, &data);
	}
	// system("leaks --quiet minishell");
	return (SUCCESS);
}

static void	manage_signals(void)
{
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

static void	finish_line(char *str, t_data *data)
{
	free(str);
	free_arr((void **)data->cmd_arr);
	unlink_heredoc(data->head);
	lst_tree_free_all(data->head);
	data->head = 0;
	system("leaks --quiet minishell");
}
