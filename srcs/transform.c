/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 09:01:28 by rolee             #+#    #+#             */
/*   Updated: 2023/03/04 19:04:26 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "parse.h"

#include "util.h"

// cd 명령에서 expand해줘야 할 게 있을 수도(~, -)
// bonus에서 * 와일드카드

void	transform(t_data *data, t_pipe *info)
{
	t_token	*search;
	int		is_expanded;

	search = info->head;
	while (search)
	{
		is_expanded = 0;
		search->token = expand(data, search->token, &is_expanded);
		search->token = remove_quote(search->token);
		if (is_expanded && ft_strlen(search->token) == 0)
		{
			pull_token(info->head, search);
			info->token_arr_size--;
		}
		search = search->right;
	}
}

// void	transform(t_data *data, t_pipe *info)
// {
// 	int	idx;
// 	int	is_expanded;

// 	idx = 0;
// 	while (info->token_arr[idx])
// 	{
// 		is_expanded = 0;
// 		info->token_arr[idx] = expand(data, info->token_arr[idx], &is_expanded);
// 		info->token_arr[idx] = remove_quote(info->token_arr[idx]);
// 		if (is_expanded && ft_strlen(info->token_arr[idx]) == 0)
// 		{
// 			pull_token(info->token_arr, idx);
// 			info->token_arr_size--;
// 		}
// 		idx++;
// 	}
// }
