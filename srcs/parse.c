/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 20:47:06 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/25 16:52:54 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"

int	parse_line(char *str, t_pipe *info)
{
	//공백으로 스플릿, 리다이렉션&파이프는 따로 저장?, 따옴표 안의 공백 유지
	//파이프가 보이면 pipe_size 추가
	tokenalize(info);
	// 파이프가 맨 앞뒤에 있을 때, 정의되지 않은 기호들 파이프 & 라디이렉션
	// 리다이렉션 뒤에 기호가 올 떄
	if (chk_grammer_valid(info) < 0)
		return (-1);
	transform(info);
	// 환경변수, 따옴표 제거
	// ls -l > outfile | cat -e "$FILE  asd"
	// ls / -l / > / outfile / | / 
	// cat / -e / "$FILE  asd"
	return (0);
}