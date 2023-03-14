/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_func2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 17:47:57 by rolee             #+#    #+#             */
/*   Updated: 2023/03/14 18:00:14 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

static void	remove_env(t_data *data, char *str);
static int	is_number(char *str);

int	builtin_unset(t_data *data, char **cmd_arr)
{
	int		idx;

	if (!cmd_arr[1])
		return (EXIT_SUCCESS);
	idx = 1;
	while (cmd_arr[idx])
		remove_env(data, cmd_arr[idx++]);
	return (EXIT_SUCCESS);
}

static void	remove_env(t_data *data, char *str)
{
	t_env	*env;
	t_env	*prev;

	env = data->env;
	prev = env;
	while (env)
	{
		if (ft_strncmp(env->name, str, ft_strlen(str) + 1) == 0)
		{
			if (data->env == env)
			{
				data->env = env->next;
				lst_env_free(env);
				env = data->env;
				continue ;
			}
			prev->next = env->next;
			lst_env_free(env);
			env = prev;
		}
		prev = env;
		env = env->next;
	}
}

int	builtin_exit(char **cmd_arr)
{
	if (cmd_arr[1])
	{
		if (!is_number(cmd_arr[1]))
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(cmd_arr[1], STDERR_FILENO);
			ft_putendl_fd(": numeric argument required", STDERR_FILENO);
			exit(255);
		}
		if (cmd_arr[2])
		{
			ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
			return (EXIT_FAILURE);
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
