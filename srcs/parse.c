/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 20:47:06 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/05 11:27:41 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"
#include "parse.h"

int			chk_condition(t_token *now, int *brace_opened);
static int	chk_grammer_valid(t_pipe *info);

int	parse_line(char *str, t_data *data, t_pipe *info)
{
	if (tokenize(str, info) == FAILURE || chk_grammer_valid(info) == FAILURE)
	{
		if (info->head == 0)
			return (FAILURE);
		// free_list(head);
		return (FAILURE);
	}
	transform(data, info);
	// make_tree(info);
	return (SUCCESS);
}

static int	chk_grammer_valid(t_pipe *info)
{
	t_token	*search;
	int		brace_opened;

	search = info->head;
	brace_opened = 0;
	if (!search)
		return (FAILURE);
	while (search)
	{
		if (chk_condition(search, &brace_opened) == FAILURE)
		{
			printf("syntax error near unexpected token '%s'\n", search->token);
			return (FAILURE);
		}
		if (search->type == BRACE && search->token[0] == '(')
			brace_opened++;
		search = search->right;
	}
	return (SUCCESS);
}

int	chk_condition(t_token *now, int *brace_opened)
{
	if (now->type == PIPE && (!now->left || !now->right
			|| is_symbol(now->left->token)))
		return (FAILURE);
	else if (now->type == REDIRECTION
		&& (!now->right || is_symbol(now->right->token)))
		return (FAILURE);
	else if (now->type == BRACE && now->token[0] == ')')
		return (--(*brace_opened));
	else if (!ft_strncmp(now->token, "&", 2))
		return (FAILURE);
	return (SUCCESS);
}
