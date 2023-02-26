/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_rm_quote.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 18:15:17 by rolee             #+#    #+#             */
/*   Updated: 2023/02/26 14:01:17 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

static char	*remove_quote(char *str);
static int	get_close_idx(int idx, char *str, char quote);
static char	*get_str_without_quote(int open_idx, int close_idx, char *str);

// cd : . / .. / ~ / -
// bonus : *은 와일드카드로 인식하기

void	transform(char **token_arr)
{
	int	idx;

	idx = 0;
	while (token_arr[idx])
	{
		token_arr[idx] = expand_env(token_arr[idx]);
		token_arr[idx] = remove_quote(token_arr[idx]);
		idx++;
	}
}

static char	*remove_quote(char *str)
{
	int		idx;
	int		open_idx;
	int		close_idx;
	char	quote;

	idx = 0;
	while (str[idx])
	{
		if (str[idx] == '\'' || str[idx] == '\"')
		{
			quote = str[idx];
			open_idx = idx;
			close_idx = get_close_idx(idx + 1, str, quote);
			if (close_idx == -1)
				return (str);
			str = get_str_without_quote(open_idx, close_idx, str);
			idx += close_idx - open_idx;
		}
		idx++;
	}
	return (str);
}

static int	get_close_idx(int idx, char *str, char quote)
{
	while (str[idx] != quote && str[idx])
		idx++;
	if (!str[idx])
		return (-1);
	return (idx);
}

static char	*get_str_without_quote(int open_idx, int close_idx, char *str)
{
	int	new_size;
	char *new_str;
	int	str_idx;
	int	new_idx;

	new_size = ft_strlen(str) - 2;
	new_str = (char *)malloc(new_size + 1);
	if (!new_str)
		exit(EXIT_FAILURE);
	str_idx = 0;
	new_idx = 0;
	while (str[str_idx])
	{
		if (str_idx != open_idx && str_idx != close_idx)
			new_str[new_idx++] = str[str_idx];
		str_idx++;
	}
	new_str[new_idx] = 0;
	free(str);
	return (new_str);
}
