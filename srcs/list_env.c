/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 17:51:03 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/18 15:10:38 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

t_env	*lst_new_env(char *name, char *value)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		exit (EXIT_FAILURE);
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

void	lst_env_free_all(t_env *lst)
{
	t_env	*search;
	t_env	*next;

	search = lst;
	while (search)
	{
		next = search->next;
		lst_env_free(search);
		search = next;
	}
}
