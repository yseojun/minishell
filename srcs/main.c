/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 14:25:04 by seojyang          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/02/18 19:18:49 by seojyang         ###   ########.fr       */
=======
/*   Updated: 2023/02/18 19:19:25 by rolee            ###   ########.fr       */
>>>>>>> d3faab6487f62a88d9d204ba87659e600b3fc928
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
			// < 가 들어왔다면 : 다음 파일이 존재하는지 확인하고 stdin으로 연결 (인덱스 두 번 넘기기)
			// > 가 들어왔다면 : 다음 파일을 stdout으로 연결 (인덱스 두 번 넘기기)
			// | 가 들어왔다면 : 파이프 READ_END를 stdout에 연결
			// command가 들어왔다면 : 이후의 인덱스를 보면서 stdin, stdout 연결
				// > 또는 >> 또는 더 이상 없을 때까지 while문 돌면서 확인
					// < 가 있다면 그 다음 파일이 존재하는지 확인하고 stdin으로 연결
					// > 또는 >> 가 있다면 그 다음 파일을 stdout으로 연결
					// | 가 있다면 파이프 READ_END를 stdout으로 연결
					// 더이상 없다면 stdout을 stdout으로 연결
				// 실행하기⭐️
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
