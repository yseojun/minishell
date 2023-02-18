/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 14:01:55 by seojyang          #+#    #+#             */
/*   Updated: 2022/11/11 21:39:57 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	idx;
	char	*tmp;

	idx = ft_strlen(s);
	tmp = (char *) s;
	if (c == 0)
		return (&tmp[idx]);
	while (idx >= 0)
	{
		if ((unsigned char) tmp[idx] == (unsigned char) c)
			return (&tmp[idx]);
		if (idx == 0)
			break ;
		idx--;
	}
	return (0);
}
