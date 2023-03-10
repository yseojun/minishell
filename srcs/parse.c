/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 20:47:06 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/14 15:46:33 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"
#include "parse.h"

static void	token_error(char *token);
static int	chk_condition(t_token *now, int *brace_opened);
static int	chk_grammer_valid(t_data *data);

int	parse_line(char *str, t_data *data)
{
	if (tokenize(str, data) == FAILURE || chk_grammer_valid(data) == FAILURE
		|| transform(data) == FAILURE)
	{
		if (data->head == 0)
			return (FAILURE);
		lst_token_free_all(data->head);
		return (FAILURE);
	}
	// 타입이 리다이렉션이고, 와일드 카드의 결과가 여러 개면 오류(ambiguous redirect)
	// 왼쪽이 히어독 리다이렉션일 때 와일드 카드 하지 말기
	//token_prt(data->head);
	data->head = make_tree(lst_token_last(data->head));
	//printf("%p\n", data->head);
	//prt_tree(data->head);
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
	// token_prt(search);
	while (search)
	{
		if (chk_condition(search, &brace_opened) == FAILURE)
		{
			token_error(search->token);
			return (FAILURE);
		}
		if (search->type == REDIRECTION)
			search = search->right;
		if (search->type == BRACE && search->token[0] == '(')
			brace_opened++;
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

static void	token_error(char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
	ft_putstr_fd(token, 2);
	ft_putendl_fd("'", 2);
}
