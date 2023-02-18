/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 14:08:29 by seojyang          #+#    #+#             */
/*   Updated: 2022/11/13 19:23:57 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	size_t	idx;

	if (!dst && !src)
		return (0);
	idx = 0;
	if (dst > src)
	{
		while (idx < n)
		{
			*((unsigned char *) dst + n - 1 - idx) = \
			*((unsigned char *) src + n - 1 - idx);
			idx++;
		}
	}
	else
		ft_memcpy(dst, src, n);
	return (dst);
}
