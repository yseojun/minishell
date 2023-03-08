/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_func.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojun <seojun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 18:07:22 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/08 15:41:56 by seojun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"

static void	_env(t_data *data);
static void	_export(t_data *data, char *token);
static void	_unset(t_data *data, char *name);
static void builtin_cd(char *dir, t_data *data);
static void	builtin_pwd(t_data *data);
static void	builtin_echo(t_pipe *info);

void	run_builtin_func(t_pipe *info, t_data *data)
{
	if (info->is_built_in == EXPORT)
		_export(data, info->cmd_arr[1]);
	else if (info->is_built_in == ENV)
		_env(data);
	else if (info->is_built_in == UNSET)
		_unset(data, info->cmd_arr[1]);
	else if (info->is_built_in == EXIT)
		builtin_exit(EXIT_SUCCESS, data);
	else if (info->is_built_in == CD)
		builtin_cd(info->cmd_arr[1], data);
	else if (info->is_built_in == PWD)
		builtin_pwd(data);
	if (info->is_built_in == _ECHO)
		builtin_echo(info);
	// if (info->is_built_in == HISTORY)
	// 	printf("history 실행");
}

static void	_env(t_data *data)
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

static void	_export(t_data *data, char *token)
{
	t_env	*search;
	char	**name_val;

	name_val = ft_split(token, '=');
	// chk_valid_export(name_val);
	search = data->env;
	while (search)
	{
		printf("%s, %s\n", search->name, name_val[0]);
		if (ft_strncmp(search->name, name_val[0], ft_strlen(name_val[0]) + 1) == 0)
		{
			free(search->value);
			search->value = name_val[1];
			break ;
		}
		search = search->next;
	}
	if (search == NULL)
		lst_env_add_back(&data->env, lst_new_env(name_val[0], name_val[1]));
	free_arr((void **) name_val);
}

static void	_unset(t_data *data, char *name)
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

void	builtin_exit(int status, t_data *data)
{
	data->exit_status = status;
	exit(status);
}

static void builtin_cd(char *dir, t_data *data)
{
	// 오류 처리
	// 존재하지 않은 디렉토리 또는 파일인 경우
	// 디렉토리가 아닌 경우

	if (chdir(dir) == FAILURE)
	{
		perror("minishell: cd");
		builtin_exit(EXIT_FAILURE, data); // 1 말고 다른 값으로 exit 해야 하나?	
	}
	printf("dir: %s", getcwd(NULL, 0));
}

static void	builtin_pwd(t_data *data)
{
	char	*curr_dir;

	curr_dir = getcwd(NULL, 0);
	if (!curr_dir)
		builtin_exit(EXIT_FAILURE, data);
	ft_putendl_fd(curr_dir, STDOUT_FILENO);
	free(curr_dir);
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

static void	builtin_echo(t_pipe *info)
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
}
