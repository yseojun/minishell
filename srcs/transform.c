/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 09:01:28 by rolee             #+#    #+#             */
/*   Updated: 2023/03/18 20:57:26 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "parse.h"
#include "util.h"

static int		is_todo_wildcard(t_token *search);
static int		is_surrounded_by_quote(char *token);

int	transform(t_data *data)
{
	t_token	*search;
	int		is_expanded;

	search = data->head;
	while (search)
	{
		is_expanded = 0;
		search->token = expand(data, search->token, &is_expanded);
		if (is_todo_wildcard(search))
		{
			if (wildcard(data, &search) == FAILURE)
				return (FAILURE);
			continue ;
		}
		search->token = remove_quote(search->token);
		if (is_expanded && ft_strlen(search->token) == 0)
			search->type = ERROR;
		search = search->right;
	}
	return (SUCCESS);
}

static int	is_todo_wildcard(t_token *search)
{
	if (is_surrounded_by_quote(search->token))
		return (FALSE);
	if (search->left && ft_strncmp(search->left->token, "<<", 3) == 0)
		return (FALSE);
	return (TRUE);
}

static int	is_surrounded_by_quote(char *token)
{
	int	single_flag;
	int	double_flag;

	single_flag = 0;
	double_flag = 0;
	while (*token)
	{
		if (*token == '\'')
			single_flag = single_flag != 1;
		else if (*token == '\"')
			double_flag = double_flag != 1;
		else if (*token == '*' && !single_flag && !double_flag)
			return (FALSE);
		token++;
	}
	return (TRUE);
}
