/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:40:21 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/19 19:21:28 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"
#include "parse.h"

static void	run_line(t_data *data);

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
			run_line(&data);
		finish_line(str, &data);
	}
	lst_env_free_all(data.env);
	return (SUCCESS);
}

static void	run_line(t_data *data)
{
	exit_status(EXIT_SUCCESS);
	find_heredoc(data->head);
	if (exit_status(LOAD) == EXIT_SUCCESS)
		execute_tree(data->head, data);
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
