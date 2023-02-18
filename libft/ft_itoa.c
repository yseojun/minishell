/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 19:01:01 by seojyang          #+#    #+#             */
/*   Updated: 2022/11/11 21:05:09 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	chk_len(long long n)
{
	size_t	len;

	len = 0;
	if (n == 0)
		return (1);
	else if (n < 0)
		len++;
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char		*output;
	size_t		len;
	long long	num;

	num = n;
	len = chk_len(num);
	output = (char *)malloc(len + 1);
	if (output)
	{
		output[len] = '\0';
		if (num < 0)
		{
			output[0] = '-';
			num = -num;
		}
		if (num == 0)
			output[0] = '0';
		while (len-- > 0 && num)
		{
			output[len] = num % 10 + '0';
			num /= 10;
		}
	}
	return (output);
}
