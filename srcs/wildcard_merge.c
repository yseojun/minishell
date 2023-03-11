/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_merge.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 13:00:21 by lru0409           #+#    #+#             */
/*   Updated: 2023/03/11 14:45:16 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

static int		count_wildcard_lst(t_wildcard *wildcard);
static t_token	*wildcard_merge(t_pipe *info, t_token *now);

int	merge_wildcard_lst(t_pipe *info, t_token **now)
{
	int		count;
	t_token	*temp;

	count = count_wildcard_lst(info->wildcard);
	if ((*now)->type == REDIRECTION && count > 1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd((*now)->token, STDERR_FILENO);
		ft_putendl_fd(": ambiguous redirect", STDERR_FILENO);
		return (FAILURE);
	}
	temp = *now;
	*now = wildcard_merge(info, *now);
	lst_token_free(temp);
	lst_wildcard_free_all(info->wildcard);
	info->wildcard = 0;
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

static t_token	*wildcard_merge(t_pipe *info, t_token *now)
{
	t_token		*merge_head;
	t_token		*add;
	t_wildcard	*search;	

	merge_head = 0;
	search = info->wildcard;
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
		info->head = merge_head;
	add->right = now->right;
	if (now->right)
		now->right->left = add;
	return (now->right);
}
