/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 09:01:28 by rolee             #+#    #+#             */
/*   Updated: 2023/03/18 11:56:27 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "parse.h"
#include "util.h"

static int		is_todo_wildcard(t_token *search);
static int		is_surrounded_by_quote(char *token);
static t_token	*pull_token(t_token **head, t_token *remove);

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
			search = pull_token(&data->head, search);
		else
			search = search->right;
	}
	if (!data->head)
		return (FAILURE);
	return (SUCCESS);
}

static t_token	*pull_token(t_token **head, t_token *remove)
{
	t_token	*temp;

	temp = remove->right;
	if (remove->left)
		remove->left->right = remove->right;
	if (remove->right)
	{
		if (*head == remove)
			*head = remove->right;
		remove->right->left = remove->left;
	}
	if (!remove->left && !remove->right)
		*head = 0;
	lst_token_free(remove);
	remove = NULL;
	return (temp);
}

static int	is_todo_wildcard(t_token *search)
{
	if (ft_strchr(search->token, '*') == 0)
		return (FALSE);
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
