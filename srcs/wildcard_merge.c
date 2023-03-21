/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_merge.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 13:00:21 by lru0409           #+#    #+#             */
/*   Updated: 2023/03/21 20:57:04 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

static void		wildcard_merge(t_data *data, t_token *now);
static t_token	*make_merge_node(char *name, t_token *now);

int	merge_wildcard_lst(t_data *data, t_token **now)
{
	wildcard_merge(data, *now);
	lst_wildcard_free_all(data->wildcard);
	data->wildcard = 0;
	return (SUCCESS);
}

static void	wildcard_merge(t_data *data, t_token *now)
{
	t_token		*now_right;
	t_token		*add;
	t_wildcard	*search;

	search = data->wildcard;
	add = 0;
	while (search)
	{
		if (search == data->wildcard)
		{
			add = make_merge_node(search->name, now);
			lst_token_last(now)->right = add;
		}
		else
		{
			if (search->next != 0)
			{
				now_right = now->right;
				now->right = 0;
			}
			free(now->token);
			now->token = ft_strdup(search->name);
		}
		search = search->next;
	}
	if (add)
		add->right = now_right;
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
