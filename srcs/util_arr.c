/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_arr.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 20:51:25 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/12 13:41:47 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"
#include "base.h"

int	free_arr(void **arr) // 리턴 void?
{
	int	i;

	if (!arr)
		return (SUCCESS);
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
	return (SUCCESS);
}

int	chk_arr_size(char **arr)
{
	int	idx;

	idx = 0;
	while (arr[idx])
		idx++;
	return (idx);
}

void	prt_arr(char **arr) //테스트 용도. 마지막에 지우자 
{
	while (*arr)
	{
		ft_putendl_fd(*arr, 2);
		arr++;
	}
}
