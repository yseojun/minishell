/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:40:21 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/19 18:03:57 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "pipe.h"

int	chk_tmp_size(char **tmp);
void	free_arr(void **arr);

void	chk_ascii(char *str)
{
	int idx = 0;
	
	while (str[idx])
	{
		printf("%d\n", str[idx]);
		idx++;
	}
}

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
		str = readline("minishell> ");
		if (!str) // exit, ctrl C ctrl D
			break ;
		add_history(str);
		// rl_redisplay();
		// chk_ascii(str);
		str = ft_strtrim(str, "\n");
		pipe_info.tmp = ft_split(str, '|');
		pipe_info.tmp_size = chk_tmp_size(pipe_info.tmp);
		pipe_info.prev_fd = 0;
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
