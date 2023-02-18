/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 16:24:21 by seojyang          #+#    #+#             */
/*   Updated: 2022/11/11 21:27:03 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	unsigned char	*new;

	if (size != 0 && count > SIZE_MAX / size)
		return (0);
	new = (unsigned char *)malloc(count * size);
	if (new)
		ft_bzero(new, count * size);
	return (new);
}
