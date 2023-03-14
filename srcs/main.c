/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:40:21 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/14 16:31:46 by seojyang         ###   ########.fr       */
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
		return (FAILURE);
	set_beginning(&data, envp);
	while (1)
	{
		reset_line_data(&data);
		str = readline("minishell> ");
		if (!str)
			break ;
		add_history(str);
		if (parse_line(str, &data) == SUCCESS)
		{
			exit_status(0);
			find_heredoc(data.head);
			if (exit_status(LOAD) == EXIT_SUCCESS)
				excute_tree(data.head, &data);
		}
		finish_line(str, &data);
	}
	return (SUCCESS);
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
