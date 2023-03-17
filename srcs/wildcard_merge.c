/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_merge.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 13:00:21 by lru0409           #+#    #+#             */
/*   Updated: 2023/03/17 20:09:49 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

static int		count_wildcard_lst(t_wildcard *wildcard);
static t_token	*wildcard_merge(t_data *data, t_token *now);

int	merge_wildcard_lst(t_data *data, t_token **now)
{
	int		count;
	t_token	*temp;

	count = count_wildcard_lst(data->wildcard);
	if ((*now)->type == REDIRECTION && count > 1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd((*now)->token, STDERR_FILENO);
		ft_putendl_fd(": ambiguous redirect", STDERR_FILENO);
		return (FAILURE);
	}
	temp = *now;
	*now = wildcard_merge(data, *now);
	lst_token_free(temp);
	lst_wildcard_free_all(data->wildcard);
	data->wildcard = 0;
	return (SUCCESS);
}

static int	count_wildcard_lst(t_wildcard *wildcard)
{
	int			count;
	t_wildcard	*search;

	count = 0;
	search = wildcard;
	while (search)
	{
		count++;
		search = search->next;
	}
	return (count);
}

static t_token	*wildcard_merge(t_data *data, t_token *now)
{
	t_token		*merge_head;
	t_token		*add;
	t_wildcard	*search;	

	merge_head = 0;
	search = data->wildcard;
	while (search)
	{
		add = (t_token *)malloc(sizeof(t_token));
		add->token = ft_strdup(search->name);
		add->type = now->type;
		add->left = 0;
		add->right = 0;
		lst_token_add_back(&merge_head, add);
		search = search->next;
	}
	merge_head->left = now->left;
	if (now->left)
		now->left->right = merge_head;
	else
		data->head = merge_head;
	add->right = now->right;
	if (now->right)
		now->right->left = add;
	return (now->right);
}
