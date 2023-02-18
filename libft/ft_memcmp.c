/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 14:07:01 by seojyang          #+#    #+#             */
/*   Updated: 2022/11/13 19:22:22 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned int	idx;
	unsigned char	*m1;
	unsigned char	*m2;

	if (!n)
		return (0);
	m1 = (unsigned char *) s1;
	m2 = (unsigned char *) s2;
	idx = 0;
	while (idx + 1 < n)
	{
		if (m1[idx] != m2[idx])
			break ;
		idx++;
	}
	return (m1[idx] - m2[idx]);
}
