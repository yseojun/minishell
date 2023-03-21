/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_pipefd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 21:41:15 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/21 20:51:50 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

t_pipefd	*lst_new_pipefd(int fd[2])
{
	t_pipefd	*new;

	new = (t_pipefd *)malloc(sizeof(t_pipefd));
	if (!new)
		exit (EXIT_FAILURE);
	new->pipefd[0] = fd[0];
	new->pipefd[1] = fd[1];
	new->next = NULL;
	return (new);
}

void	lst_pipefd_remove_last(t_pipefd **head)
{
	t_pipefd	*search;
	t_pipefd	*tmp;

	if (!*head)
		return ;
	search = *head;
	if (!search->next)
	{
		free(search);
		*head = NULL;
		return ;
	}
	while (search->next->next)
		search = search->next;
	tmp = search->next;
	search->next = NULL;
	free(tmp);
}

void	lst_pipefd_add_back(t_pipefd **head, t_pipefd *new)
{
	t_pipefd	*last;

	if (!new)
		return ;
	last = lst_pipefd_last(*head);
	if (!last)
		*head = new;
	else
		last->next = new;
}

t_pipefd	*lst_pipefd_last(t_pipefd *lst)
{
	t_pipefd	*search;

	if (!lst)
		return (0);
	search = lst;
	while (search->next)
		search = search->next;
	return (search);
}

void	lst_pipefd_free_all(t_pipefd *lst)
{
	t_pipefd	*search;
	t_pipefd	*tmp;

	if (!lst)
		return ;
	search = lst;
	while (search)
	{
		tmp = search->next;
		free(search);
		search = tmp;
	}
}
