/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 20:47:06 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/28 20:57:00 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"
#include "parse.h"

static int	chk_grammer_valid(t_pipe *info);

int	parse_line(char *str, t_data *data, t_pipe *info)
{
	// 파이프가 맨 앞뒤에 있을 때, 정의되지 않은 기호들 파이프 & 라디이렉션
	// 리다이렉션 뒤에 기호가 올 떄
	if (tokenalize(str, info) == FAILURE || chk_grammer_valid(info) == FAILURE)
	{
		if (info->token_arr == 0)
			return (FAILURE);
		free_arr((void **) info->token_arr);
		return (FAILURE);
	}
	transform(data, info);
	return (SUCCESS);
}

static int	chk_grammer_valid(t_pipe *info)
{
	int	idx;

	idx = 1;
	if (!info->token_arr || !info->token_arr[0])
		return (FAILURE);
	if (is_pipe(info->token_arr[0]))
		return (FAILURE);
	while (info->token_arr[idx])
	{
		if (is_symbol(info->token_arr[idx])
			&& is_symbol(info->token_arr[idx - 1]))
		{
			printf("syntax error near unexpected token '%s'\n", info->token_arr[idx]);
			return (FAILURE);
		}
		// chk_grammer(info->token_arr[idx]);
		idx++;
	}
	if (ft_strncmp(info->token_arr[idx - 1], "|", 2) == 0)
		return (FAILURE);
	return (SUCCESS);
}
