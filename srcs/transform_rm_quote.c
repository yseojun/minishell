/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_rm_quote.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 09:09:21 by rolee             #+#    #+#             */
/*   Updated: 2023/03/18 11:50:39 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

static int	get_close_idx(int idx, char quote, char *str);
static char	*rm_quote(int open_idx, int close_idx, char *str);

char	*remove_quote(char *str)
{
	int		idx;
	int		open_idx;
	int		close_idx;

	idx = 0;
	while (str[idx])
	{
		if (str[idx] == '\'' || str[idx] == '\"')
		{
			open_idx = idx;
			close_idx = get_close_idx(idx + 1, str[idx], str);
			if (close_idx == FAILURE)
			{
				idx++;
				continue ;
			}
			str = rm_quote(open_idx, close_idx, str);
			idx += close_idx - open_idx - 2;
		}
		idx++;
	}
	return (str);
}

static int	get_close_idx(int idx, char quote, char *str)
{
	while (str[idx] != quote && str[idx])
		idx++;
	if (!str[idx])
		return (FAILURE);
	return (idx);
}

static char	*rm_quote(int open_idx, int close_idx, char *str)
{
	int		new_size;
	char	*new_str;
	int		idx;
	int		new_idx;

	new_size = ft_strlen(str) - 2;
	new_str = (char *)malloc(new_size + 1);
	if (!new_str)
		exit(EXIT_FAILURE);
	idx = 0;
	new_idx = 0;
	while (str[idx])
	{
		if (idx != open_idx && idx != close_idx)
			new_str[new_idx++] = str[idx];
		idx++;
	}
	new_str[new_idx] = 0;
	free(str);
	return (new_str);
}
