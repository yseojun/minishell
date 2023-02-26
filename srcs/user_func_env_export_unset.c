/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_func_env_export_unset.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 18:07:22 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/26 19:09:43 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"

void	run_user_func(t_pipe *info, t_data *data)
{
	if (info->is_built_in == 1)
		_export(data, info->cmd_arr[1]);
	if (info->is_built_in == 2)
		_unset(data, info->cmd_arr[1]);
	if (info->is_built_in == 3)
		exit(0);
	// else if (ft_strncmp(info->cmd_arr[0], "cd", 2) == 0)
	// 	_cd(info->cmd_arr[1]);
	// else if (ft_strncmp(info->cmd_arr[0], "pwd", 3) == 0)
	// 	_pwd(info->cmd_arr[1]);
	// else if (ft_strncmp(info->cmd_arr[0], "echo", 4) == 0)
	// 	_echo(info->cmd_arr[1]);
	// else if (ft_strncmp(info->cmd_arr[0], "history", 7) == 0)
	// 	_history(info->cmd_arr[1]);
}

char	*get_env(t_data *data, char *str)
{
	t_env	*search;

	search = data->env;
	if (ft_strncmp(str, "?", 2) == 0)
		return (ft_itoa(data->last_child_status));
	while (search)
	{
		if (ft_strncmp(search->name, str, ft_strlen(str)) == 0)
			return (search->value);
		search = search->next;
	}
	return (ft_strdup(""));
}

void	_env(t_data *data)
{
	t_env	*search;

	search = data->env;
	while (search)
	{
		ft_putstr_fd(search->name, 1);
		ft_putchar_fd('=', 1);
		ft_putendl_fd(search->value, 1);
		search = search->next;
	}
}

void	_export(t_data *data, char *token)
{
	t_env	*search;
	t_env	*prev;
	char	**name_val;

	name_val = ft_split(token, '=');
	// chk_valid_export(name_val);
	search = data->env;
	prev = search;
	while (search)
	{
		if (ft_strncmp(search->name, name_val[0], ft_strlen(name_val[0])) == 0)
		{
			free(search->value);
			search->value = name_val[1];
			break ;
		}
		prev = search;
		search = search->next;
	}
	lst_env_add_back(&data->env, lst_new_env(name_val[0], name_val[1]));
	free_arr((void **) name_val);
}

void	_unset(t_data *data, char *name)
{
	t_env	*search;
	t_env	*prev;

	search = data->env;
	prev = search;
	while (search)
	{
		if (ft_strncmp(search->name, name, ft_strlen(name)) == 0)
		{
			if (prev == search)
				data->env = search->next;
			else
				prev->next = search->next;
			lst_env_free(search);
			return ;
		}
		prev = search;
		search = search->next;
	}
}
