/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 20:24:02 by seojyang          #+#    #+#             */
/*   Updated: 2022/11/13 19:21:44 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*new_elem;
	t_list	*search;

	if (!lst || !f || !del)
		return (0);
	search = lst;
	new_list = 0;
	while (search)
	{
		new_elem = ft_lstnew(f(search->content));
		if (!new_elem)
		{
			ft_lstclear(&new_list, del);
			return (new_list);
		}
		ft_lstadd_back(&new_list, new_elem);
		search = search->next;
	}
	return (new_list);
}
