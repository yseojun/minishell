/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 14:03:57 by seojyang          #+#    #+#             */
/*   Updated: 2022/11/11 15:47:45 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*tmp;

	if ((char) c == '\0')
		return ((char *) s + ft_strlen(s));
	tmp = (char *) s;
	while (*tmp)
	{
		if ((unsigned char) *tmp == (unsigned char) c)
			return (tmp);
		tmp++;
	}
	return (0);
}
