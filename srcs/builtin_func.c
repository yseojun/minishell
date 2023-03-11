/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_func.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 18:07:22 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/11 19:20:33 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"

static int	builtin_env(char **cmd_arr, t_data *data);
static void	chk_valid_export(char *str, int *exit_status);
static int	is_number(char *str);
static int	builtin_pwd(void);
static int	builtin_echo(t_pipe *info);

int	run_builtin_func(t_pipe *info, t_data *data)
{
	if (info->is_built_in == EXPORT)
		return (builtin_export(data, info->cmd_arr));
	else if (info->is_built_in == ENV)
		return (builtin_env(info->cmd_arr, data));
	else if (info->is_built_in == UNSET)
		return (builtin_unset(data, info->cmd_arr));
	else if (info->is_built_in == EXIT)
		return (builtin_exit(info->cmd_arr));
	else if (info->is_built_in == CD)
		return (builtin_cd(info->cmd_arr[1]));
	else if (info->is_built_in == PWD)
		return (builtin_pwd());
	else
		return (builtin_echo(info));
}

static int	builtin_env(char **cmd_arr, t_data *data)
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
	t_env	*env;
	char	**name_val;
	
	if (!cmd_arr[1])
		return (builtin_env(cmd_arr, data));
	exit_status = EXIT_SUCCESS;
	idx = 1;
	prt_arr(cmd_arr);
	while (cmd_arr[idx])
	{
		if (ft_strchr(cmd_arr[idx], '='))
		{
			chk_valid_export(cmd_arr[idx], &exit_status);
			name_val = ft_split(cmd_arr[idx], '=');
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
		idx++;
	}
	return (exit_status);
}

static void	chk_valid_export(char *str, int *exit_status)
{
	if (str[0] == '=')
	{
		*exit_status = EXIT_FAILURE;
		ft_putstr_fd("minishell: export: ", STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putendl_fd(": not a valid identifier", STDERR_FILENO);	
	}
}

int	builtin_unset(t_data *data, char **cmd_arr)
{
	int		idx;
	t_env	*env;
	t_env	*prev;

	if (!cmd_arr[1])
		return (EXIT_SUCCESS);
	idx = 1;
	while (cmd_arr[idx])
	{
		env = data->env;
		prev = env;
		while (env)
		{
			if (ft_strncmp(env->name, cmd_arr[idx], ft_strlen(cmd_arr[idx])) == 0)
			{
				if (data->env == env)
				{
					data->env = env->next;
					lst_env_free(env);
					env = data->env;
					continue;
				}
				prev->next = env->next;
				lst_env_free(env);
				env = prev;
			}
			prev = env;
			env = env->next;
		}
		idx++;
	}
	return (EXIT_SUCCESS);
}

int	builtin_exit(char **cmd_arr)
{
	if (cmd_arr[1] && cmd_arr[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (cmd_arr[1])
	{
		if (!is_number(cmd_arr[1]))
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(cmd_arr[1], STDERR_FILENO);
			ft_putendl_fd(": numeric argument required", STDERR_FILENO);
			exit(255);
		}
		exit((unsigned char)ft_atoi(cmd_arr[1]));
	}
	exit(EXIT_SUCCESS);
}

static int	is_number(char *str)
{
	int	idx;

	idx = 0;
	if (str[0] == '-' || str[0] == '+')
		idx++;
	while (str[idx])
	{
		if (!ft_isdigit(str[idx]))
			return (FALSE);
		idx++;
	}
	return (TRUE);
}

int	builtin_cd(char *dir) // ~ 처리
{
	if (chdir(dir) == FAILURE)
	{
		perror("minishell: cd");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	builtin_pwd(void)
{
	char	*curr_dir;

	curr_dir = getcwd(NULL, 0);
	if (!curr_dir)
		return (EXIT_FAILURE);
	ft_putendl_fd(curr_dir, STDOUT_FILENO);
	free(curr_dir);
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

static int	builtin_echo(t_pipe *info)
{
	int	remove_nl;
	int	idx;

	remove_nl = is_n_option(info->cmd_arr[1]);
	if (remove_nl == TRUE)
		idx = 2;
	else
		idx = 1;
	while (info->cmd_arr[idx])
	{
		ft_putstr_fd(info->cmd_arr[idx], STDOUT_FILENO);
		if (info->cmd_arr[idx + 1] != NULL)
			ft_putchar_fd(' ', STDOUT_FILENO);
		idx++;
	}
	if (remove_nl == FALSE)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
