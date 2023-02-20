/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 18:07:22 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/20 09:09:03 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

void	init_data(t_data *data)
{
	data->now = 0;
	data->status = 0;
	data->my_env_fd = open("./srcs/mnsh_env.txt", \
					O_CREAT | O_RDWR | O_TRUNC, 0644);
}

char	*get_env(char *str)
{
	int	idx;

	idx = 0;
	if (str == 0)
	{
		while (environ[idx])
			printf("%s\n", environ[idx++]);
		return (0);
	}
	else
	{
		while (environ[idx])
		{
			if (ft_strncmp(str, environ[idx], ft_strlen(str)) == 0)
				return (environ[idx] + ft_strlen(str) + 1);
			idx++;
		}
		return (0);
	}
}

int	_export(char *to_add)
{
	//char	**new;
	int		idx;

	idx = 0;
	while (environ[idx])
		idx++;
	environ[idx] = to_add; //멀록안해도 되면 이걸로
	environ[idx + 1] = 0;
	// new = (char **)malloc(sizeof(char *) * (idx + 1));
	// idx = 0;
	// while (environ[idx])
	// {
	// 	new[idx] = ft_strdup(environ[idx]);
	// 	idx++;
	// }
	return (0);
}

int	_unset(char *to_del)
{
	int	idx;

	idx = 0;
	while (environ[idx])
	{
		if (ft_strncmp(to_del, environ[idx], ft_strlen(environ[idx]) == 0))
		{
			environ[idx++] = 0;
			while (environ[idx])
			{
				environ[idx - 1] = environ[idx];
				idx++;
			}
			environ[idx - 1] = 0;
		}
		idx++;
	}
	return (0);
}
