/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 09:33:45 by rolee             #+#    #+#             */
/*   Updated: 2023/03/11 16:15:18 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_word_count(char const *s, char c)
{
	int	flag;
	int	idx;
	int	count;

	flag = 0;
	idx = 0;
	count = 0;
	if (!s)
		return (0);
	while (s[idx])
	{
		if (s[idx] == c)
			flag = 0;
		else
		{
			if (flag == 0)
				count++;
			flag = 1;
		}
		idx++;
	}
	return (count);
}

static int	get_word_len(char const *s, char c)
{
	int	idx;
	int	len;

	idx = 0;
	while (s[idx] == c)
		idx++;
	len = 0;
	while (s[idx] != c && s[idx])
	{
		len++;
		idx++;
	}
	return (len);
}

static int	alloc_put_word(char **result, char const *s, char c, int i)
{
	int	idx;

	result[i] = (char *)malloc(get_word_len(s, c) + 1);
	if (!result[i])
	{
		idx = 0;
		while (idx < i)
		{
			free(result[idx]);
			idx++;
		}
		free(result);
		return (-1);
	}
	idx = 0;
	while (s[idx] == c)
		idx++;
	while (s[idx] != c && s[idx])
	{
		result[i][idx] = s[idx];
		idx++;
	}
	result[i][idx] = 0;
	return (0);
}

static int	set_result(char **result, char const *s, char c)
{
	int	word_idx;
	int	str_idx;

	str_idx = 0;
	if (!s)
	{
		result[0] = 0;
		return (0);
	}
	while (s[str_idx] == c && s[str_idx])
		str_idx++;
	word_idx = 0;
	while (word_idx < get_word_count(s, c))
	{
		if (alloc_put_word(result, &s[str_idx], c, word_idx) == -1)
			return (-1);
		while (s[str_idx] != c && word_idx + 1 != get_word_count(s, c))
			str_idx++;
		while (s[str_idx] == c && word_idx + 1 != get_word_count(s, c))
			str_idx++;
		word_idx++;
	}
	result[word_idx] = 0;
	return (0);
}

char	**ft_split(char const *s, char c)
{
	int		word_count;
	char	**result;

	word_count = get_word_count(s, c);
	if (!word_count)
		return (0);
	result = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (!result)
		return (NULL);
	if (set_result(result, s, c) == -1)
		return (NULL);
	return (result);
}
