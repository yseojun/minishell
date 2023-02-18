/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 14:25:04 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/18 19:18:49 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "pipe.h"

int	main(void)
{
	char	*str;
	int		i;
	t_data	data;

	while (1)
	{
		i = 0;
		ft_putstr_fd("minishell> ", 1);
		str = get_next_line(0);
		if (!str) // exit, ctrl C ctrl D
			break ;
		data.tmp = ft_split(str, ' ');
		data.tmp_size = chk_tmp_size(data.tmp);
		while (i < data.tmp_size)
		{
			data.command = get_command(data.tmp);
			run_tmp(&data); // pipe exit하면 어떻게?
			i++;
		}
		free(str);
		free_arr(data.tmp);
		str = 0;
	}
}

void	free_arr(void **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}
