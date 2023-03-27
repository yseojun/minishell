/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_beginning.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 21:11:24 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/27 20:19:30 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"

static t_env	*init_env(char *envp[]);
static void		level_up(t_data *data);

void	set_beginning(t_data *data, char *envp[])
{
	data->env = init_env(envp);
	level_up(data);
	tcgetattr(0, &data->term);
	data->term.c_lflag &= ~ECHOCTL;
	tcsetattr(0, TCSANOW, &data->term);
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
}

static void	level_up(t_data *data)
{
	t_env	*search;
	char	*num;

	search = data->env;
	while (search)
	{
		if (ft_strncmp(search->name, "SHLVL", 6) == 0)
			break ;
		search = search->next;
	}
	if (search)
	{
		if (is_number(search->value))
			num = ft_itoa(ft_atoi(search->value) + 1);
		else
			num = ft_strdup("1");
		free(search->value);
		search->value = num;
	}
	else
		lst_env_add_back(&data->env, lst_new_env("SHLVL", "1"));
}

void	handler(int sig)
{
	if (sig == SIGINT)
	{
		exit_status(256 * EXIT_FAILURE);
		ft_putchar_fd('\n', STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
		ft_putendl_fd("Quit: 3", STDERR_FILENO);
}

static t_env	*init_env(char *envp[])
{
	t_env	*head;
	char	**env_spl;
	int		idx;

	idx = 0;
	head = 0;
	while (envp[idx])
	{
		env_spl = ft_split(envp[idx], '=');
		lst_env_add_back(&head, lst_new_env(env_spl[0], env_spl[1]));
		free_arr((void **)env_spl);
		idx++;
	}
	return (head);
}

void	heredoc_handler(int sig)
{
	if (sig == SIGINT)
		ft_putchar_fd('\n', STDOUT_FILENO);
}
