/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenalize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 19:14:42 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/25 21:22:21 by seojyang         ###   ########.fr       */
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
		return (-1);
	n = 0;
	idx = 0;
	while (str[idx])
	{
		if (str[idx] == ' ')
			idx++;
		else
		{
			if (!put_token(str + idx, new_arr, n++, &idx))
				return (-1);
		}
	}
	new_arr[n] = 0;
	info->token_arr = new_arr;
	info->token_arr_size = n;
	return (0);
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
	int		token_size;

	token_size = count_quote(str, *idx);
	*idx += token_size;
	new[n] = (char *)malloc(token_size + 1);
	if (!new[n])
		return (0);
	i = 0;
	while (i < token_size)
	{
		new[n][i] = str[i];
		i++;
	}
	new[n][i] = '\0';
	return (1);
}