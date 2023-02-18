/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 16:53:24 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/18 15:11:54 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	size;

	size = len;
	if (!s)
		return (0);
	if (ft_strlen(s) < (size_t) start)
		size = 0;
	else if (ft_strlen(s + start) <= len)
		size = ft_strlen(s + start);
	str = (char *)malloc(size + 1);
	if (!str)
		return (0);
	if (size)
		ft_memcpy((void *) str, (void *) s + start, size);
	str[size] = '\0';
	return (str);
}
