/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chk_condition.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 12:15:14 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/26 12:51:34 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "parse.h"

int	chk_if_open_brace(t_token *now, int *brace_opened)
{
	if (now->type == BRACE && now->token[0] == '(')
	{
		(*brace_opened)++;
		if (now->left)
		{
			if (now->left->type == CMD)
				return (FAILURE);
			else if (now->left->type == PIPE)
				return (FAILURE);
		}
	}
	return (SUCCESS);
}

int	chk_if_close_brace(t_token *now, int *brace_opened)
{
	if (now->type == BRACE && now->token[0] == ')')
	{
		(*brace_opened)--;
		if (now->left)
		{
			if (now->left->type == BRACE && now->left->token[0] == '(')
				return (FAILURE);
			else if (is_symbol(now->left->token))
				return (FAILURE);
			else if (now->left->type == AND || now->left->type == OR)
				return (FAILURE);
		}
	}
	return (SUCCESS);
}

int	chk_if_pipe_logic(t_token *now)
{
	if (now->type == PIPE || now->type == AND || now->type == OR)
	{
		if (!now->left || !now->right)
			return (FAILURE);
		else if (is_symbol(now->left->token))
			return (FAILURE);
		else if (now->left->type == AND || now->left->type == OR)
			return (FAILURE);
		else if (now->left->type == BRACE && now->left->token[0] == '(')
			return (FAILURE);
		else if (now->right->type == BRACE && now->right->token[0] == ')')
			return (FAILURE);
	}
	return (SUCCESS);
}

int	chk_if_redirection(t_token *now)
{
	if (now->type == REDIRECTION)
	{
		if (!now->right || is_redirection(now->right->token))
			return (FAILURE);
		else if (now->left->type == BRACE && now->left->token[0] == ')')
			return (FAILURE);
	}
	return (SUCCESS);
}

int	chk_if_cmd(t_token *now)
{
	if (now->type == CMD)
	{
		if (now->left)
		{
			if (now->left->type == BRACE && now->left->token[0] == ')')
				return (FAILURE);
		}
	}
	return (SUCCESS);
}
