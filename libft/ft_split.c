/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 21:19:56 by seojyang          #+#    #+#             */
/*   Updated: 2022/11/11 21:39:01 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_sep(char check, char c)
{
	return (check == c);
}

static int	count_str(char *str, char c)
{
	int	count;

	count = 0;
	while (*str)
	{
		if (is_sep(*str, c))
		{
			while (is_sep(*str, c))
				str++;
		}
		else if (!is_sep(*str, c))
		{
			while (*str && !is_sep(*str, c))
				str++;
			count++;
		}
	}
	return (count + 1);
}

static int	split_put(char *str, char c, char **output, int n)
{
	int	idx;

	idx = 0;
	while (str[idx] && !is_sep(str[idx], c))
		idx++;
	output[n] = (char *)malloc(idx + 1);
	if (!output[n])
		return (0);
	idx = 0;
	while (str[idx] && !is_sep(str[idx], c))
	{
		output[n][idx] = str[idx];
		idx++;
	}
	output[n][idx] = '\0';
	return (1);
}

static char	**free_split(char **output)
{
	size_t	idx;

	idx = 0;
	while (output[idx])
		free(output[idx++]);
	free(output);
	return (0);
}

char	**ft_split(char const *s, char c)
{
	char	**output;
	int		n;
	int		idx;

	n = 0;
	idx = 0;
	if (!s)
		return (0);
	output = (char **)malloc(sizeof(char *) * count_str((char *) s, c));
	if (!output)
		return (0);
	while (s[idx])
	{
		if (is_sep(s[idx], c))
			idx++;
		else
		{
			if (!split_put((char *) s + idx, c, output, n++))
				return (free_split(output));
			while (s[idx] && !is_sep(s[idx], c))
				idx++;
		}
	}
	output[n] = 0;
	return (output);
}
