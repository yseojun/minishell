/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_check_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 16:06:05 by rolee             #+#    #+#             */
/*   Updated: 2023/03/28 11:15:41 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "parse.h"
#include "util.h"

static int	check_cmd_path(t_data *data);

int	check_cmd(t_data *data, t_token *unit)
{
	char	*path_value;

	if (data->cmd_arr == 0)
		exit(SUCCESS);
	if (data->is_built_in)
		return (SUCCESS);
	if (unit->type == ERROR && unit->right == NULL)
		return (SUCCESS);
	if (data->is_built_in)
		return (SUCCESS);
	if (check_cmd_path(data) == SUCCESS)
		return (SUCCESS);
	if (ft_strchr(data->cmd_arr[0], '/')
		&& access(data->cmd_arr[0], F_OK) == SUCCESS)
		return (SUCCESS);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(data->cmd_arr[0], STDERR_FILENO);
	path_value = get_value(data, "PATH");
	if (path_value)
		ft_putendl_fd(": command not found", STDERR_FILENO);
	else
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	free(path_value);
	exit(127);
	return (FAILURE);
}

void	is_builtin_func(t_data *data)
{
	if (data->cmd_arr == 0)
		return ;
	if (ft_strncmp(data->cmd_arr[0], "export", 7) == 0)
		data->is_built_in = EXPORT;
	else if (ft_strncmp(data->cmd_arr[0], "env", 4) == 0)
		data->is_built_in = ENV;
	else if (ft_strncmp(data->cmd_arr[0], "unset", 6) == 0)
		data->is_built_in = UNSET;
	else if (ft_strncmp(data->cmd_arr[0], "exit", 5) == 0)
		data->is_built_in = EXIT;
	else if (ft_strncmp(data->cmd_arr[0], "cd", 3) == 0)
		data->is_built_in = CD;
	else if (ft_strncmp(data->cmd_arr[0], "pwd", 4) == 0)
		data->is_built_in = PWD;
	else if (ft_strncmp(data->cmd_arr[0], "echo", 5) == 0)
		data->is_built_in = _ECHO;
	else
		data->is_built_in = 0;
}

static int	check_cmd_path(t_data *data)
{
	char	**paths;
	int		idx;
	char	*tmp_path;

	paths = get_paths(data);
	idx = 0;
	while (paths && paths[idx])
	{
		tmp_path = make_real_path(paths[idx], data->cmd_arr[0]);
		if (access(tmp_path, F_OK) == SUCCESS)
		{
			free(tmp_path);
			free_arr((void **)paths);
			return (SUCCESS);
		}
		idx++;
		free(tmp_path);
	}
	if (paths)
		free_arr((void **)paths);
	return (FAILURE);
}

char	**get_paths(t_data *data)
{
	char	*path_value;
	char	**paths;

	path_value = get_value(data, "PATH");
	if (!path_value)
		return (NULL);
	paths = ft_split(path_value, ':');
	free(path_value);
	if (!paths)
		exit(EXIT_FAILURE);
	return (paths);
}

char	*make_real_path(char *path, char *command)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*str;

	if (!path || !command)
		return (NULL);
	s1_len = ft_strlen(path);
	s2_len = ft_strlen(command);
	str = (char *)malloc(s1_len + s2_len + 2);
	if (!str)
		return (NULL);
	ft_memcpy(str, path, s1_len);
	str[s1_len] = '/';
	ft_memcpy(str + s1_len + 1, command, s2_len + 1);
	return (str);
}
