/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 20:47:06 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/14 20:49:40 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"
#include "parse.h"

static void	remove_comment(char *str);
static void	token_error(char *token);
static int	chk_condition(t_token *now, int *brace_opened);
static int	chk_grammer_valid(t_data *data);

int	parse_line(char *str, t_data *data)
{
	remove_comment(str);
	if (tokenize(str, data) == FAILURE || chk_grammer_valid(data) == FAILURE
		|| transform(data) == FAILURE)
	{
		if (data->head == 0)
			return (FAILURE);
		lst_token_free_all(data->head);
		return (FAILURE);
	}
	data->head = make_tree(lst_token_last(data->head));
	return (SUCCESS);
}

static int	chk_grammer_valid(t_data *data)
{
	t_token	*search;
	int		brace_opened;

	search = data->head;
	brace_opened = 0;
	if (!search)
		return (FAILURE);
	while (search)
	{
		if (chk_condition(search, &brace_opened) == FAILURE)
		{
			token_error(search->token);
			return (FAILURE);
		}
		if (search->type == REDIRECTION)
			search = search->right;
		search = search->right;
	}
	if (brace_opened > 0)
	{
		token_error("(");
		return (FAILURE);
	}
	return (SUCCESS);
}

static int	chk_condition(t_token *now, int *brace_opened)
{
	if (now->type == BRACE && now->token[0] == '(')
		(*brace_opened)++;
	if ((now->type == PIPE || now->type == AND || now->type == OR)
		&& (!now->left || !now->right || is_symbol(now->left->token)))
		return (FAILURE);
	else if (now->type == REDIRECTION
		&& (!now->right || is_symbol(now->right->token)))
		return (FAILURE);
	else if (now->type == BRACE && now->token[0] == ')')
	{
		if ((now->left && now->left->type == BRACE
				&& now->left->token[0] == '(') || *brace_opened <= 0)
			return (FAILURE);
		return (--(*brace_opened));
	}
	else if (!ft_strncmp(now->token, "&", 2))
		return (FAILURE);
	return (SUCCESS);
}


static void	remove_comment(char *str)
{
	while (*str)
	{
		if (*str == '#')
			*str = 0;
		str++;
	}
}

static void	token_error(char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
	ft_putstr_fd(token, 2);
	ft_putendl_fd("'", 2);
}
