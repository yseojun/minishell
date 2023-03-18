/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:40:21 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/18 15:11:00 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"
#include "parse.h"

static void	run_line(t_data *data);
static void	finish_line(char *str, t_data *data);
static void	lst_tree_free_all(t_token *top);

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
		excute_tree(data->head, data);
}

static void	finish_line(char *str, t_data *data)
{
	free(str);
	free_arr((void **)data->cmd_arr);
	unlink_heredoc(data->head);
	lst_tree_free_all(data->head);
	data->head = 0;
	//system("leaks --quiet minishell");
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

static void	lst_tree_free_all(t_token *top)
{
	if (top == 0)
		return ;
	lst_tree_free_all(top->left);
	lst_tree_free_all(top->right);
	lst_token_free(top);
}
