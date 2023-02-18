/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 14:07:51 by seojyang          #+#    #+#             */
/*   Updated: 2022/11/11 20:57:39 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	*ft_memset(void *b, int c, int len)
{
	unsigned char	*tmp;

	tmp = (unsigned char *) b;
	while (len--)
		*tmp++ = (unsigned char) c;
	return (b);
}
