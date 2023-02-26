/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenalize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 19:14:42 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/26 14:19:23 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

static int		count_quote(char *str, int idx);
static size_t	count_arr_size(char *str);
static int		put_token(char *str, char **new, int n, int *idx);

int	tokenalize(char *str, t_pipe *info)
{
	char	**new_arr;
	int		idx;
	int		n;

	new_arr = (char **)malloc(sizeof(char *) * count_arr_size(str));
	if (!new_arr)
		return (FAILURE);
	n = 0;
	idx = 0;
	while (str[idx])
	{
		if (str[idx] == ' ')
			idx++;
		else
		{
			if (put_token(str, new_arr, n++, &idx) == FAILURE)
				return (FAILURE);
		}
	}
	new_arr[n] = 0;
	info->token_arr = new_arr;
	info->token_arr_size = n;
	return (SUCCESS);
}

static int	count_quote(char *str, int idx)
{
	int		quote_size;
	char	qoute;

	quote_size = 0;
	qoute = str[idx];
	idx++;
	quote_size++;
	while (str[idx] && str[idx] != qoute)
	{
		idx++;
		quote_size++;
	}
	return (quote_size);
}

static size_t	count_arr_size(char *str)
{
	size_t	count;
	int		idx;

	count = 0;
	idx = 0;
	while (str[idx])
	{
		if (str[idx] == ' ')
		{
			while (str[idx] == ' ')
				idx++;
		}
		else
		{
			while (str[idx] && str[idx] != ' ')
			{
				if (str[idx] == '\'' || str[idx] == '\"')
					idx += count_quote(str, idx);
				idx++;
			}
			count++;
		}
	}
	return (count + 1);
}

static int	put_token(char *str, char **new, int n, int *idx)
{
	int		i;
	int		start;
	int		token_size;

	token_size = 0;
	start = *idx;
	while (str[*idx] && str[*idx] != ' ')
	{
		if (str[*idx] == '\'' || str[*idx] == '\"')
		{
			token_size += count_quote(str, *idx);
			*idx += token_size;
		}
		(*idx)++;
		token_size++;
	}
	new[n] = (char *)malloc(token_size + 1);
	if (!new[n])
		return (FAILURE);
	i = 0;
	while (i < token_size)
	{
		new[n][i] = str[start + i];
		i++;
	}
	new[n][i] = '\0';
	return (SUCCESS);
}
