/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 14:06:48 by seojyang          #+#    #+#             */
/*   Updated: 2022/11/13 19:28:03 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	m;

	m = 0;
	if (!n)
		return (0);
	while (m + 1 < n && s1[m] && s2[m])
	{
		if (s1[m] != s2[m])
			break ;
		m++;
	}
	return ((unsigned char)s1[m] - (unsigned char)s2[m]);
}
