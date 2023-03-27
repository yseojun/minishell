/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_make2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 13:18:45 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/26 14:26:34 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "parse.h"

static t_token	*set_cmd_top(t_token *head);
static t_token	*add_command(t_token *top, t_token *add);
static t_token	*add_redirection(t_token **top, t_token *add);

t_token	*find_command(t_token *tail)
{
	t_token	*head;
	t_token	*search;
	t_token	*top;

	head = tail;
	while (head->left)
		head = head->left;
	top = set_cmd_top(head);
	search = head;
	while (search)
	{
		if (search->type == CMD)
			search = add_command(top, search);
		else if (search->type == REDIRECTION)
			search = add_redirection(&top, search);
	}
	return (top);
}

static t_token	*set_cmd_top(t_token *head)
{
	t_token	*search;
	t_token	*top;

	search = head;
	while (search)
	{
		if (search->type == CMD)
			break ;
		search = search->right;
	}
	top = search;
	if (top)
		top->left = 0;
	return (top);
}

static t_token	*add_command(t_token *top, t_token *add)
{
	t_token	*next_search;
	t_token	*search;

	next_search = add->right;
	if (next_search)
	{
		next_search->left->right = NULL;
		next_search->left = NULL;
	}
	if (top == add)
		return (next_search);
	search = top;
	while (search->right)
		search = search->right;
	search->right = add;
	return (next_search);
}

static t_token	*add_redirection(t_token **top, t_token *add)
{
	t_token	*next_search;
	t_token	*search;

	next_search = add->right->right;
	if (next_search)
	{
		if (next_search->left != add->right)
			add->right->right = 0;
		else
		{
			next_search->left->right = NULL;
			next_search->left = NULL;
		}
	}
	if (*top)
	{
		search = *top;
		while (search->left)
			search = search->left;
		search->left = add;
	}
	else
		*top = add;
	add->right->left = 0;
	return (next_search);
}
