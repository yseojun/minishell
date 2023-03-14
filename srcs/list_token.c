/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 19:08:44 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/14 16:32:33 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

t_token	*make_token(char *str, int *idx)
{
	t_token	*new;

	new = (t_token *)malloc(sizeof(t_token));
	new->left = 0;
	new->right = 0;
	new->token = put_token(str, idx);
	new->type = set_type(new->token);
	return (new);
}

int	lst_token_add_back(t_token **head, t_token *new)
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
	lst = 0;
}

void	lst_token_free_all(t_token *lst)
{
	t_token	*search;
	t_token	*next;

	search = lst;
	while (search)
	{
		next = search->right;
		lst_token_free(search);
		search = next;
	}
}

void	lst_tree_free_all(t_token *top)
{
	if (top == 0)
		return ;
	lst_tree_free_all(top->left);
	lst_tree_free_all(top->right);
	lst_token_free(top);
}
