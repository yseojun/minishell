/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:40:21 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/19 16:41:16 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "pipe.h"

int	chk_tmp_size(char **tmp);
void	free_arr(void **arr);

int	main(void)
{
	char	*str;
	int		i;
	int		test;
	// t_data	data;
	t_pipe	pipe_info;

	init_info(&pipe_info);
	test = 0;
	while (1)
	{
		ft_putstr_fd("minishell> ", 1);
		str = get_next_line(0);
		if (!str) // exit, ctrl C ctrl D
			break ;
		str = ft_strtrim(str, "\n");
		pipe_info.tmp = ft_split(str, '|');
		pipe_info.tmp_size = chk_tmp_size(pipe_info.tmp);
		i = 0;
		while (i < pipe_info.tmp_size)
		{
			pipe_info.infile_fd = 0;
			pipe_info.outfile_fd = 1;
			if (run_pipe(&pipe_info, i) < 0)
				break ;
			i++;
		}
		wait_all(&pipe_info);
		free(str);
		free(pipe_info.tmp);
		test++;
	}
}

int	chk_tmp_size(char **tmp)
{
	int	idx;

	idx = 0;
	while (tmp[idx])
		idx++;
	return (idx);
}

void	free_arr(void **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}
