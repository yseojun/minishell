/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_wildcard.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 20:26:35 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/18 12:44:56 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

t_wildcard	*lst_new_wildcard(char *name)
{
	t_wildcard	*new;

	new = (t_wildcard *)malloc(sizeof(t_wildcard));
	if (!new)
		exit (EXIT_FAILURE);
	new->name = ft_strdup(name);
	new->next = 0;
	return (new);
}

int	wildcard_add_back(t_wildcard **head, t_wildcard *new)
{
	t_wildcard	*last;

	if (!new)
		return (0);
	last = lst_wildcard_last(*head);
	if (!last)
		*head = new;
	else
		last->next = new;
	return (1);
}

t_wildcard	*lst_wildcard_last(t_wildcard *lst)
{
	t_wildcard	*search;

	if (!lst)
		return (0);
	search = lst;
	while (search->next)
		search = search->next;
	return (search);
}

void	lst_wildcard_free(t_wildcard *lst)
{
	free(lst->name);
	free(lst);
	lst = 0;
}

void	lst_wildcard_free_all(t_wildcard *lst)
{
	t_wildcard	*search;
	t_wildcard	*next;

	search = lst;
	while (search)
	{
		next = search->next;
		lst_wildcard_free(search);
		search = next;
	}
}
