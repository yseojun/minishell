/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenalize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 19:14:42 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/28 13:45:57 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

static int		get_quote_size(char *str, int idx);
static size_t	get_arr_size(char *str);
static int		put_token(char *str, char **new, int n, int *idx);

int	tokenalize(char *str, t_pipe *info)
{
	char	**new_arr;
	int		idx;
	int		n;

	new_arr = (char **)malloc(sizeof(char *) * get_arr_size(str));
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
			if (put_token(str, new_arr, n, &idx) == FAILURE)
				return (FAILURE);
			n++;
		}
	}
	new_arr[n] = 0;
	info->token_arr = new_arr;
	info->token_arr_size = n;
	return (SUCCESS);
}

static int	get_quote_size(char *str, int idx)
{
	int		quote_size;
	char	quote;

	quote_size = 0;
	quote = str[idx];
	idx++;
	quote_size++;
	while (str[idx] && str[idx] != quote)
	{
		idx++;
		quote_size++;
	}
	if (!str[idx])
		return (0);
	return (quote_size);
}

static size_t	get_arr_size(char *str)
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
				if (is_redir_pipr_chr(str[idx]))
				{
					if (str[idx + 1] == str[idx])
						idx++;
					idx++;
					break ;
				}
				else if (str[idx] == '\'' || str[idx] == '\"')
					idx += get_quote_size(str, idx);
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
		if (is_redir_pipr_chr(str[*idx]))
		{
			if (str[*idx + 1] == str[*idx])
			{
				token_size++;
				(*idx)++;
			}
			(*idx)++;
			token_size++;
			break ;
		}
		else if (str[*idx] == '\'' || str[*idx] == '\"')
		{
			token_size += get_quote_size(str, *idx);
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
