/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_func3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 17:51:38 by rolee             #+#    #+#             */
/*   Updated: 2023/03/27 20:29:33 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "parse.h"
#include "util.h"

static int	is_n_option(char **str);
static int	_chdir(char *dir);

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
		if (_chdir(dir) == EXIT_FAILURE)
		{
			free(dir);
			return (EXIT_FAILURE);
		}
		free(dir);
	}
	else
	{
		if (_chdir(dir) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	_chdir(char *dir)
{
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

	remove_nl = is_n_option(data->cmd_arr);
	idx = remove_nl + 1;
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

static int	is_n_option(char **str)
{
	int	idx;
	int	count;

	count = 0;
	while (str[count + 1])
	{
		if (str[count + 1][0] != '-')
			return (count);
		idx = 1;
		while (str[count + 1][idx])
		{
			if (str[count + 1][idx] != 'n')
				return (count);
			idx++;
		}
		count++;
	}
	return (count);
}
