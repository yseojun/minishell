/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 20:12:27 by seojyang          #+#    #+#             */
/*   Updated: 2022/11/11 15:22:22 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*search;
	t_list	*save;

	if (!lst || !del)
		return ;
	search = *lst;
	while (search)
	{
		save = search->next;
		ft_lstdelone(search, del);
		search = save;
	}
	*lst = 0;
}
