/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_make.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 12:36:39 by seojun            #+#    #+#             */
/*   Updated: 2023/03/14 18:05:27 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"

static int			check_wildcard(char *name, char **to_find, t_token *now);
static int			is_good_format(char *name, char **to_find, char *token);
static int			find_str(char *to_find_str, char *name, int *idx);
static t_wildcard	*remove_wildcard(t_data *data, t_wildcard *prev, \
	t_wildcard *to_remove);

void	cmp_wildcard(t_data *data, char **to_find, t_token *now)
{
	t_wildcard	*search;
	t_wildcard	*prev;

	if (!data->wildcard)
		return ;
	search = data->wildcard;
	prev = search;
	while (search)
	{
		if (check_wildcard(search->name, to_find, now) == FAILURE)
			search = remove_wildcard(data, prev, search);
		else
		{
			prev = search;
			search = search->next;
		}
	}
}

static int	check_wildcard(char *name, char **to_find, t_token *now)
{
	int	idx;
	int	to_find_idx;

	idx = 0;
	to_find_idx = 0;
	if (is_good_format(name, to_find, now->token))
	{
		while (to_find[to_find_idx])
		{
			if (find_str(to_find[to_find_idx], name, &idx) == FAILURE)
				break ;
			to_find_idx++;
		}
		if (to_find[to_find_idx] == 0)
			return (SUCCESS);
	}
	return (FAILURE);
}

static int	is_good_format(char *name, char **to_find, char *token)
{
	int			find_len;
	const int	name_len = ft_strlen(name);

	find_len = 0;
	if (token[0] != '*')
	{
		find_len = ft_strlen(*to_find);
		if (ft_strncmp(name, *to_find, find_len) != 0)
			return (0);
	}
	while (*(to_find + 1))
		to_find++;
	if (token[ft_strlen(token) - 1] != '*')
	{
		find_len = ft_strlen(*to_find);
		if (name_len < find_len
			|| ft_strncmp(name + name_len - find_len, *to_find, find_len) != 0)
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
