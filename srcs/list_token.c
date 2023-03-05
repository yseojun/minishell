/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 19:08:44 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/05 15:25:18 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

int	token_add_back(t_token **head, t_token *new)
{
	t_token	*last;

	if (!new)
		return (0);
	last = lst_token_last(*head);
	if (!last)
		*head = new;
	else
		last->right = new;
	new->left = last;
	return (1);
}

t_token	*lst_token_last(t_token *lst)
{
	t_token	*search;

	if (!lst)
		return (0);
	search = lst;
	while (search->right)
		search = search->right;
	return (search);
}

void	lst_token_free(t_token *lst)
{
	free(lst->token);
	free(lst);
}

void	token_prt(t_token *head)
{
	t_token	*search;

	search = head;
	while (search)
	{
		printf("token : %s type : %d\n", search->token, search->type);
		search = search->right;
	}
}

void	prt_tree(t_token *head)
{
	if (head == 0)
		return ;
	printf("token : %s, type : %d\n", head->token, head->type);
	prt_tree(head->left);
	prt_tree(head->right);
}
