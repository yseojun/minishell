/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 20:47:06 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/26 21:32:09 by seojyang         ###   ########.fr       */
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
	int	idx;

	idx = 0;
	while (str[idx])
	{
		if (str[idx] == '#')
			str[idx] = 0;
		idx++;
	}
	if (tokenize(str, data) == FAILURE || chk_grammer_valid(data) == FAILURE)
	{
		lst_token_free_all(data->head);
		data->head = 0;
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
			exit_status(258);
			return (FAILURE);
		}
		search = search->right;
	}
	if (brace_opened > 0)
	{
		ft_putendl_fd("minishell: brace unclosed", 2);
		return (FAILURE);
	}
	return (SUCCESS);
}

static int	chk_condition(t_token *now, int *brace_opened)
{
	if (chk_if_open_brace(now, brace_opened) == FAILURE)
		return (FAILURE);
	else if (chk_if_close_brace(now, brace_opened) == FAILURE)
		return (FAILURE);
	else if (chk_if_pipe_logic(now) == FAILURE)
		return (FAILURE);
	else if (chk_if_redirection(now) == FAILURE)
		return (FAILURE);
	else if (chk_if_cmd(now) == FAILURE)
		return (FAILURE);
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
