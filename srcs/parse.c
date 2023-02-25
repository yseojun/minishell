/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 20:47:06 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/25 20:11:21 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"

int	parse_line(char *str, t_pipe *info)
{
	//공백으로 스플릿, 리다이렉션&파이프는 따로 저장?, 따옴표 안의 공백 유지
	//파이프가 보이면 pipe_size 추가

	// 파이프가 맨 앞뒤에 있을 때, 정의되지 않은 기호들 파이프 & 라디이렉션
	// 리다이렉션 뒤에 기호가 올 떄
	if (tokenalize(str, info) < 0 || chk_grammer_valid(info) < 0)
	{
		if (info->token_arr == 0)
			return (-1);
		free_arr(info->token_arr);
		return (-1);
	}
	transform(info->token_arr);
	// 환경변수, 따옴표 제거
	// ls -l > outfile | cat -e "$FILE  asd"
	// ls / -l / > / outfile / | / 
	// cat / -e / "$FILE  asd"
	return (0);
}

int	chk_grammer_valid(t_pipe *info)
{
	int	idx;

	idx = 1;
	if (!info->token_arr)
		return (-1);
	if (is_pipe(info->token_arr[0]))
		return (-1);
	while (info->token_arr[idx])
	{
		if (is_symbol(info->token_arr[idx])
			&& is_redirection(info->token_arr[idx - 1]))
			return (-1);
		// chk_grammer(info->token_arr[idx]);
		idx++;
	}
	if (ft_strncmp(info->token_arr[idx - 1], "|", 2) == 0)
		return (-1);
	return (0);
}
