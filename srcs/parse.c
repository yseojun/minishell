/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 20:47:06 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/04 18:43:08 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"
#include "parse.h"

static int	chk_grammer_valid(t_pipe *info);

int	parse_line(char *str, t_data *data, t_pipe *info)
{
	if (tokenize(str, info) == FAILURE || chk_grammer_valid(info) == FAILURE)
	{
		if (info->token_arr == 0)
			return (FAILURE);
		free_arr((void **) info->token_arr);
		return (FAILURE);
	}
	transform(data, info);
	// make_tree(info);
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
		// if (is_redirection(info->token_arr[idx]))
		// {
		// 	if (is_redirection(info->token_arr[idx - 1]))
		// 		return (FAILURE);
		// 	if (info->token_arr[idx + 1] == 0)
		// 		return (FAILURE);
		// 	if (is_redirection(info->token_arr[idx + 1]))
		// 		return (FAILURE);
		// 	idx++;
		// }
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
