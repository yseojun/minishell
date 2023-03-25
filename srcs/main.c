/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:40:21 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/25 19:50:47 by rolee            ###   ########.fr       */
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
	if (find_heredoc(data->head) == EXIT_SUCCESS)
		execute_tree(data->head, data);
}

int	exit_status(int status)
{
	static int	exit_status;

	if (status == LOAD)
		return (exit_status);
	else if (status == 258)
		exit_status = 258;
	else if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		exit_status = 128 + WTERMSIG(status);
	return (exit_status);
}
