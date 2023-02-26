/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_func_env_export_unset.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 18:07:22 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/26 17:07:50 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"

void	run_user_func(t_pipe *info, t_data *data)
{
	if (info->is_built_in == 1)
		_export(info->cmd_arr[1]);
	if (info->is_built_in == 2)
		_unset(info->cmd_arr[1]);
	if (info->is_built_in == 3)
		exit(0);
	data = 0;
	// else if (ft_strncmp(info->cmd_arr[0], "cd", 2) == 0)
	// 	_cd(info->cmd_arr[1]);
	// else if (ft_strncmp(info->cmd_arr[0], "pwd", 3) == 0)
	// 	_pwd(info->cmd_arr[1]);
	// else if (ft_strncmp(info->cmd_arr[0], "echo", 4) == 0)
	// 	_echo(info->cmd_arr[1]);
	// else if (ft_strncmp(info->cmd_arr[0], "history", 7) == 0)
	// 	_history(info->cmd_arr[1]);
}

char	*get_env(char *str)
{
	int	idx;

	idx = 0;
	if (str == 0)
	{
		while (environ[idx])
			printf("%s\n", environ[idx++]);
		return (SUCCESS);
	}
	else
	{
		while (environ[idx])
		{
			if (ft_strncmp(str, environ[idx], ft_strlen(str)) == 0)
				return (environ[idx] + ft_strlen(str) + 1);
			idx++;
		}
		return (SUCCESS);
	}
}

int	_export(char *to_add)
{
	int		idx;
	char	**check;

	idx = 0;
	check = ft_split(to_add, '=');
	if (check[1] && check[0])
	{
		while (environ[idx])
		{
			if (ft_strncmp(check[0], environ[idx], ft_strlen(check[0])) == 0)
			{
				environ[idx] = to_add;
				break ;
			}
			idx++;
		}
		free_arr((void **)check);
	}
	return (SUCCESS);
}

int	_unset(char *to_del)
{
	int	idx;

	idx = 0;
	while (environ[idx])
	{
		if (ft_strncmp(to_del, environ[idx], ft_strlen(environ[idx]) == 0))
		{
			environ[idx++] = 0;
			while (environ[idx])
			{
				environ[idx - 1] = environ[idx];
				idx++;
			}
			environ[idx - 1] = 0;
		}
		idx++;
	}
	return (SUCCESS);
}
