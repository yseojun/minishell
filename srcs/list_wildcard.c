/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_wildcard.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 20:26:35 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/07 20:29:08 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

t_wildcard	*make_token(char *name)
{
	t_wildcard	*new;

	new = (t_wildcard *)malloc(sizeof(t_wildcard));
	new->name = set_type(name);
	return (new);
}

int	token_add_back(t_wildcard **head, t_wildcard *new)
{
	t_wildcard	*last;

	if (!new)
		return (0);
	last = lst_token_last(*head);
	if (!last)
		*head = new;
	else
		last->next = new;
	return (1);
}

t_wildcard	*lst_token_last(t_wildcard *lst)
{
	t_wildcard	*search;

	if (!lst)
		return (0);
	search = lst;
	while (search->next)
		search = search->next;
	return (search);
}

void	lst_token_free(t_wildcard *lst)
{
	free(lst->name);
	free(lst);
}

void	token_prt(t_wildcard *head)
{
	t_wildcard	*search;

	search = head;
	while (search)
	{
		printf("name : %s type : %d\n", search->name);
		search = search->next;
	}
}