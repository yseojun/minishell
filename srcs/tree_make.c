/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_make1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 17:07:31 by rolee             #+#    #+#             */
/*   Updated: 2023/03/26 14:47:31 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "parse.h"

static t_token	*find_logical_operator(t_token *tail);
static t_token	*find_pipe(t_token *tail);
static t_token	*ignore_brace(t_token *search);

t_token	*make_tree(t_token *tail)
{
	t_token	*top;

	top = find_logical_operator(tail);
	if (top)
		return (top);
	top = find_pipe(tail);
	if (top)
		return (top);
	top = find_braces(tail);
	if (top)
		return (top);
	top = find_command(tail);
	if (top)
		return (top);
	return (0);
}

static t_token	*find_logical_operator(t_token *tail)
{
	t_token	*search;

	search = tail;
	while (search)
	{
		if (search->type == BRACE && search->token[0] == ')')
			search = ignore_brace(search);
		else if (search->type == AND || search->type == OR)
		{
			search->left->right = NULL;
			search->left = make_tree(search->left);
			search->right->left = NULL;
			search->right = make_tree(tail);
			return (search);
		}
		search = search->left;
	}
	return (0);
}

static t_token	*find_pipe(t_token *tail)
{
	t_token	*search;

	search = tail;
	while (search)
	{
		if (search->type == BRACE && search->token[0] == ')')
			search = ignore_brace(search);
		else if (search->type == PIPE)
		{
			search->left->right = NULL;
			search->left = make_tree(search->left);
			search->right->left = NULL;
			search->right = make_tree(tail);
			return (search);
		}
		search = search->left;
	}
	return (0);
}

static t_token	*ignore_brace(t_token *search)
{
	search = search->left;
	while (search->type != BRACE || search->token[0] != '(')
	{
		if (search->type == BRACE && search->token[0] == ')')
			search = ignore_brace(search);
		search = search->left;
	}
	return (search);
}
