/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 17:51:03 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/26 19:08:59 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

t_env	*lst_new_env(char *name, char *value)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env));
	new->name = ft_strdup(name);
	new->value = ft_strdup(value);
	new->next = 0;
	return (new);
}

void	lst_env_add_back(t_env **head, t_env *new)
{
	t_env	*last;

	if (!new)
		return ;
	last = lst_env_last(*head);
	if (!last)
		*head = new;
	else
		last->next = new;
}

t_env	*lst_env_last(t_env *lst)
{
	t_env	*search;

	if (!lst)
		return (0);
	search = lst;
	while (search->next)
		search = search->next;
	return (search);
}

void	lst_env_free(t_env *lst)
{
	free(lst->name);
	free(lst->value);
	free(lst);
}
