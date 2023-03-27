/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_merge.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 13:00:21 by lru0409           #+#    #+#             */
/*   Updated: 2023/03/27 20:27:01 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

static t_token	*make_merge_node(char *name, t_token *now);
static t_token	*token_now_to_wildcard(t_wildcard *search, t_token *now);

void	wildcard_merge(t_data *data, t_token *now)
{
	t_token		*now_right;
	t_token		*add;
	t_wildcard	*search;

	search = data->wildcard;
	add = 0;
	while (search)
	{
		if (search == data->wildcard)
			now_right = token_now_to_wildcard(search, now);
		else
		{
			add = make_merge_node(search->name, now);
			lst_token_last(now)->right = add;
		}
		search = search->next;
	}
	if (add)
		add->right = now_right;
}

static t_token	*token_now_to_wildcard(t_wildcard *search, t_token *now)
{
	t_token	*now_right;

	now_right = 0;
	if (search->next != 0)
	{
		now_right = now->right;
		now->right = 0;
	}
	free(now->token);
	now->token = ft_strdup(search->name);
	return (now_right);
}

static t_token	*make_merge_node(char *name, t_token *now)
{
	t_token	*add;

	add = (t_token *)malloc(sizeof(t_token));
	if (!add)
		exit (EXIT_FAILURE);
	add->token = ft_strdup(name);
	add->type = now->type;
	add->left = 0;
	add->right = 0;
	return (add);
}
