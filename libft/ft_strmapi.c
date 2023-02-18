/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 19:11:48 by seojyang          #+#    #+#             */
/*   Updated: 2022/11/13 19:27:44 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	idx;
	char			*output;

	if (!s || !f)
		return (0);
	idx = 0;
	output = (char *)malloc(ft_strlen(s) + 1);
	if (!output)
		return (0);
	while (s[idx])
	{
		output[idx] = f(idx, s[idx]);
		idx++;
	}
	output[idx] = 0;
	return (output);
}
