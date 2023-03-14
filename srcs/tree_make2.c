/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_make2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 17:07:31 by rolee             #+#    #+#             */
/*   Updated: 2023/03/14 18:06:36 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "parse.h"

static t_token	*ignore_brace(t_token *search);

t_token	*find_logical_operator(t_token *tail)
{
	t_token	*search;

	search = tail;
	while (search)
	{
		if (search->type == BRACE)
			search = ignore_brace(search);
		else if (search->type == AND || search->type == OR)
		{
			if (search->left)
			{
				search->left->right = NULL;
				search->left = make_tree(search->left);
			}
			if (search->right)
			{
				search->right->left = NULL;
				search->right = make_tree(tail);
			}
			return (search);
		}
		search = search->left;
	}
	return (0);
}

t_token	*find_pipe(t_token *tail)
{
	t_token	*search;

	search = tail;
	while (search)
	{
		if (search->type == BRACE)
			search = ignore_brace(search);
		else if (search->type == PIPE)
		{
			if (search->left)
			{
				search->left->right = NULL;
				search->left = make_tree(search->left);
			}
			if (search->right)
			{
				search->right->left = NULL;
				search->right = make_tree(tail);
			}
			return (search);
		}
		search = search->left;
	}
	return (0);
}

static t_token	*ignore_brace(t_token *search)
{
	search = search->left;
	while (search->type != BRACE)
		search = search->left;
	return (search);
}
