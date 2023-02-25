/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_arr.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 20:51:25 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/25 17:52:12 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"

int	free_arr(void **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
	return (0);
}

int	chk_arr_size(char **arr)
{
	int	idx;

	idx = 0;
	while (arr[idx])
		idx++;
	return (idx);
}
