/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 14:09:43 by seojyang          #+#    #+#             */
/*   Updated: 2022/11/11 21:39:31 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t dstsize)
{
	size_t	idx;
	size_t	dest_len;

	idx = 0;
	dest_len = ft_strlen(dest);
	if (dest_len >= dstsize)
		return (dstsize + ft_strlen(src));
	while (src[idx] && dest_len + idx + 1 < dstsize)
	{
		dest[dest_len + idx] = src[idx];
		idx++;
	}
	dest[dest_len + idx] = '\0';
	return (dest_len + ft_strlen(src));
}
