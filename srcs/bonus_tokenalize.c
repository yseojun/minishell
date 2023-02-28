/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_tokenalize.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 13:18:45 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/28 13:28:45 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "parse.h"

int	bonus_tokenalize(char *str, t_pipe *info)
{
	t_token	*head;
	t_token	*new;
	char	**new_arr;
	int		idx;
	int		n;

	head = (t_token *)malloc(sizeof(t_token));
	head->left = 0;
	head->right = 0;
	head->token = 0;
	idx = 0;
	while (str[idx])
	{
		if (str[idx] == ' ')
			idx++;
		else
		{
			if (put_token(str, new_arr, n, &idx) == FAILURE)
				return (FAILURE);
			n++;
		}
	}
	return (SUCCESS);
}
