/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_cmp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 12:36:39 by seojun            #+#    #+#             */
/*   Updated: 2023/03/27 20:18:15 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"
#include "parse.h"

static int			chk_wildcard(char *name, t_wildcard *to_find, t_token *now);
static int			chk_ends(char *name, t_wildcard *to_find, char *token);
static int			find_str(char *to_find_str, char *name, int *idx);
static t_wildcard	*remove_wildcard(t_data *data, t_wildcard *prev, \
	t_wildcard *to_remove);

void	cmp_wildcard(t_data *data, t_wildcard *to_find, t_token *now)
{
	t_wildcard	*search;
	t_wildcard	*prev;

	if (!data->wildcard)
		return ;
	search = data->wildcard;
	prev = search;
	while (search)
	{
		if (chk_wildcard(search->name, to_find, now) == FAILURE)
			search = remove_wildcard(data, prev, search);
		else
		{
			prev = search;
			search = search->next;
		}
	}
}

static int	chk_wildcard(char *name, t_wildcard *to_find, t_token *now)
{
	t_wildcard	*search;
	int			idx;

	idx = 0;
	search = to_find;
	if (chk_ends(name, to_find, now->token))
	{
		while (search)
		{
			if (find_str(search->name, name, &idx) == FAILURE)
				break ;
			search = search->next;
		}
		if (search == 0)
			return (SUCCESS);
	}
	return (FAILURE);
}

static int	chk_ends(char *name, t_wildcard *to_find, char *token)
{
	int			find_len;
	const int	name_len = ft_strlen(name);

	find_len = 0;
	if (token[0] != '*')
	{
		find_len = ft_strlen(to_find->name);
		if (ft_strncmp(name, to_find->name, find_len) != 0)
			return (0);
	}
	while (to_find->next)
		to_find = to_find->next;
	if (token[ft_strlen(token) - 1] != '*')
	{
		find_len = ft_strlen(to_find->name);
		if (name_len < find_len
			|| ft_strncmp(name + name_len - find_len, \
				to_find->name, find_len) != 0)
			return (0);
	}
	return (1);
}

static int	find_str(char *to_find_str, char *name, int *idx)
{
	while (name[*idx])
	{
		if (ft_strncmp(name + *idx, to_find_str, ft_strlen(to_find_str)) == 0)
		{
			(*idx)++;
			return (SUCCESS);
		}
		(*idx)++;
	}
	return (FAILURE);
}

static t_wildcard	*remove_wildcard(t_data *data, t_wildcard *prev, \
	t_wildcard *to_remove)
{
	if (to_remove == data->wildcard)
	{
		data->wildcard = to_remove->next;
		lst_wildcard_free(to_remove);
		return (data->wildcard);
	}
	else
	{
		prev->next = to_remove->next;
		lst_wildcard_free(to_remove);
		return (prev->next);
	}
}
