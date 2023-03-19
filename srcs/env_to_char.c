/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_char.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 17:13:04 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/19 17:14:00 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

char	**env_to_char(t_env *env)
{
	char	**envp;
	int		count;
	t_env	*search;

	count = 0;
	search = env;
	while (search)
	{
		count++;
		search = search->next;
	}
	envp = (char **)malloc(sizeof(char *) * (count + 1));
	if (!envp)
		exit (EXIT_FAILURE);
	count = 0;
	search = env;
	while (search)
	{
		envp[count] = ft_strjoin(search->name, "=");
		envp[count] = ft_strjoin(envp[count], search->value);
		count++;
		search = search->next;
	}
	envp[count] = 0;
	return (envp);
}
