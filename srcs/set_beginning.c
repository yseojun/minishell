/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_beginning.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 21:11:24 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/17 20:21:32 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"

static t_env	*init_env(char *envp[]);

void	set_beginning(t_data *data, char *envp[])
{
	data->env = init_env(envp);
	rl_catch_signals = 0;
	tcgetattr(fileno(stdin), &data->term);
	data->term.c_lflag &= ~ECHOCTL;
	tcsetattr(fileno(stdin), TCSANOW, &data->term);
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
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
