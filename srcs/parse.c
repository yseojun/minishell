/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 20:47:06 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/21 22:02:59 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"

void	parse_line(char *str, t_pipe *info)
{
	char	*save;

	save = str;
	str = ft_strtrim(str, "\n");
	// free(save); //abortㅠㅠ
	info->tmp = ft_split(str, '|');
	info->tmp_size = chk_arr_size(info->tmp);
	info->prev_fd = 0;
}
