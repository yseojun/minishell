/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 09:01:28 by rolee             #+#    #+#             */
/*   Updated: 2023/03/12 16:19:18 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "parse.h"

#include "util.h"

// cd 명령에서 expand해줘야 할 게 있을 수도(~, -)
// bonus에서 * 와일드카드

int	transform(t_data *data, t_pipe *info)
{
	t_token	*search;
	int		is_expanded;

	search = info->head;
	while (search)
	{
		is_expanded = 0;
		search->token = expand(data, search->token, &is_expanded);
		if (!is_expanded)
		{
			make_wildcard_lst(info, search);
			if (info->wildcard)
			{
				if (merge_wildcard_lst(info, &search) == FAILURE)
					return (FAILURE);
				continue ;
			}
		}
		search->token = remove_quote(search->token);
		if (is_expanded && ft_strlen(search->token) == 0)
			search = pull_token(&info->head, search);
		else
			search = search->right;
	}
	if (info->head == 0)
		return (FAILURE);
	return (SUCCESS);
}
