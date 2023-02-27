/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 20:47:06 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/27 14:55:08 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"
#include "parse.h"

static int	chk_grammer_valid(t_pipe *info);

int	parse_line(char *str, t_data *data, t_pipe *info)
{
	//공백으로 스플릿, 리다이렉션&파이프는 따로 저장?, 따옴표 안의 공백 유지
	//파이프가 보이면 pipe_size 추가

	// 파이프가 맨 앞뒤에 있을 때, 정의되지 않은 기호들 파이프 & 라디이렉션
	// 리다이렉션 뒤에 기호가 올 떄
	if (tokenalize(str, info) == FAILURE || chk_grammer_valid(info) == FAILURE)
	{
		if (info->token_arr == 0)
			return (FAILURE);
		free_arr((void **) info->token_arr);
		return (FAILURE);
	}
	//prt_arr(info->token_arr);
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
			&& is_redirection(info->token_arr[idx - 1]))
			return (FAILURE);
		// chk_grammer(info->token_arr[idx]);
		idx++;
	}
	if (ft_strncmp(info->token_arr[idx - 1], "|", 2) == 0)
		return (FAILURE);
	return (SUCCESS);
}
