/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_func1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 18:07:22 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/19 12:35:53 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"
#include "parse.h"

static void	export_env(t_data *data, char *str);
static int	chk_export_valid(char *str);

int	run_builtin_func(t_data *data)
{
	if (data->is_built_in == EXPORT)
		return (builtin_export(data, data->cmd_arr));
	else if (data->is_built_in == ENV)
		return (builtin_env(data->cmd_arr, data));
	else if (data->is_built_in == UNSET)
		return (builtin_unset(data, data->cmd_arr));
	else if (data->is_built_in == EXIT)
		return (builtin_exit(data->cmd_arr));
	else if (data->is_built_in == CD)
		return (builtin_cd(data, data->cmd_arr[1]));
	else if (data->is_built_in == PWD)
		return (builtin_pwd());
	else
		return (builtin_echo(data));
}

int	builtin_env(char **cmd_arr, t_data *data)
{
	t_env	*search;

	if (cmd_arr[1])
	{
		ft_putendl_fd("minishell: env: invaild env arguments", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	search = data->env;
	while (search)
	{
		ft_putstr_fd(search->name, 1);
		ft_putchar_fd('=', 1);
		ft_putstr_fd(search->value, 1);
		ft_putchar_fd('\n', 1);
		search = search->next;
	}
	return (EXIT_SUCCESS);
}

int	builtin_export(t_data *data, char **cmd_arr)
{
	int		exit_status;
	int		idx;

	if (!cmd_arr[1])
		return (builtin_env(cmd_arr, data));
	exit_status = EXIT_SUCCESS;
	idx = 1;
	while (cmd_arr[idx])
	{
		if (ft_strchr(cmd_arr[idx], '='))
		{
			if (chk_export_valid(cmd_arr[idx]) == SUCCESS)
				export_env(data, cmd_arr[idx]);
			else
				exit_status = EXIT_FAILURE;
		}
		idx++;
	}
	return (exit_status);
}

static void	export_env(t_data *data, char *str)
{
	char	**name_val;
	t_env	*env;

	name_val = ft_split(str, '=');
	env = data->env;
	while (env)
	{
		if (ft_strncmp(env->name, name_val[0], ft_strlen(name_val[0]) + 1) == 0)
		{
			free(env->value);
			env->value = ft_strdup(name_val[1]);
			break ;
		}
		env = env->next;
	}
	if (!env)
		lst_env_add_back(&data->env, lst_new_env(name_val[0], name_val[1]));
	free_arr((void **) name_val);
}

static int	chk_export_valid(char *str)
{
	char	**name_val;
	int		idx;

	if (str[0] == '=')
	{
		ft_putstr_fd("minishell: export: ", STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putendl_fd(": not a valid identifier", STDERR_FILENO);
		return (FAILURE);
	}
	name_val = ft_split(str, '=');
	idx = -1;
	while (name_val[0][++idx])
	{
		if (ft_isalnum(name_val[0][idx]) == 0 && name_val[0][idx] != '_')
		{
			ft_putstr_fd("minishell: export: ", STDERR_FILENO);
			ft_putstr_fd("not valid in this context: ", STDERR_FILENO);
			ft_putendl_fd(name_val[0], STDERR_FILENO);
			free_arr((void **)name_val);
			return (FAILURE);
		}
	}
	free_arr((void **)name_val);
	return (SUCCESS);
}
