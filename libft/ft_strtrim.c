/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 17:18:14 by seojyang          #+#    #+#             */
/*   Updated: 2022/11/13 19:29:10 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*tmp;
	size_t	s1_len;
	size_t	start;
	size_t	end;

	if (!s1 || !set)
		return (0);
	s1_len = ft_strlen(s1);
	start = 0;
	end = 0;
	if (s1_len)
	{
		while (start <= s1_len && ft_strchr(set, s1[start]))
			start++;
		if (s1_len + 1 == start)
			start--;
		while (start != s1_len && ft_strchr(set, s1[s1_len - 1 - end]))
			end++;
	}
	tmp = (char *)malloc(s1_len - start - end + 1);
	if (tmp)
		ft_strlcpy(tmp, s1 + start, s1_len - start - end + 1);
	return (tmp);
}
