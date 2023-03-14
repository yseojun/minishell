/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_func3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 17:51:38 by rolee             #+#    #+#             */
/*   Updated: 2023/03/14 18:04:04 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "parse.h"
#include "util.h"

static int	is_n_option(char *str);

int	builtin_cd(t_data *data, char *dir)
{
	if (!dir)
	{
		dir = get_value(data, "HOME");
		if (!dir)
		{
			ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
	}
	if (chdir(dir) == FAILURE)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(dir);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	builtin_pwd(void)
{
	char	*curr_dir;

	curr_dir = getcwd(NULL, 0);
	if (!curr_dir)
		return (EXIT_FAILURE);
	ft_putendl_fd(curr_dir, STDOUT_FILENO);
	free(curr_dir);
	return (EXIT_SUCCESS);
}

int	builtin_echo(t_data *data)
{
	int	remove_nl;
	int	idx;

	remove_nl = is_n_option(data->cmd_arr[1]);
	if (remove_nl == TRUE)
		idx = 2;
	else
		idx = 1;
	while (data->cmd_arr[idx])
	{
		ft_putstr_fd(data->cmd_arr[idx], STDOUT_FILENO);
		if (data->cmd_arr[idx + 1] != NULL)
			ft_putchar_fd(' ', STDOUT_FILENO);
		idx++;
	}
	if (remove_nl == FALSE)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (EXIT_SUCCESS);
}

static int	is_n_option(char *str)
{
	int	idx;

	if (str[0] != '-')
		return (FALSE);
	idx = 1;
	while (str[idx])
	{
		if (str[idx] != 'n')
			return (FALSE);
		idx++;
	}
	return (TRUE);
}
