/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_make3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 14:43:35 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/26 19:52:03 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "parse.h"

static void	cut_brace_top(t_token *brace_top);

t_token	*find_braces(t_token *tail)
{
	t_token	*search;
	t_token	*brace_top;

	search = tail;
	while (search)
	{
		if (search->type == BRACE && search->token[0] == ')')
		{
			brace_top = search;
			cut_brace_top(brace_top);
			while (search->left)
				search = search->left;
			search = search->right;
			lst_token_free(search->left);
			search->left = NULL;
			brace_top->right = 0;
			brace_top->left = make_tree(lst_token_last(search));
			return (brace_top);
		}
		search = search->left;
	}
	return (0);
}

static void	cut_brace_top(t_token *brace_top)
{
	t_token	*search;

	search = brace_top;
	if (search->right)
	{
		search->left->right = search->right;
		search->right->left = search->left;
	}
	else
	{
		search = search->left;
		search->right = 0;
	}
}
