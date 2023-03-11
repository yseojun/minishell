/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 21:11:24 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/11 20:22:06 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"

static t_env	*init_env(char *envp[]);

void	init_data(t_data *data, char *envp[])
{
	data->exit_status = 0;
	data->env = init_env(envp);
	tcgetattr(0, &data->termios);
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
