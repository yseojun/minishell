/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_cmp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 12:36:39 by seojun            #+#    #+#             */
/*   Updated: 2023/03/11 12:05:55 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

static int	find_str(char *to_find_str, char *name, int *idx);
static int	check_wildcard(char *name, char **to_find);

void	cmp_wildcard(t_pipe *info, char **to_find)
{
	t_wildcard	*search;
	t_wildcard	*prev;

	if (info->wildcard == 0)
		return ;
	search = info->wildcard;
	prev = search;
	while (search)
	{
		if (check_wildcard(search->name, to_find) == -1)
			search = remove_wildcard(info, prev, search);
		search = search->next;
	}
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

static int	check_wildcard(char *name, char **to_find)
{
	int	idx;
	int	to_find_idx;

	idx = 0;
	to_find_idx = 0;
	while (to_find[to_find_idx])
	{
		if (find_str(to_find[to_find_idx], name, &idx) == FAILURE)
			break ;
		to_find_idx++;
	}
	if (to_find[to_find_idx] == 0)
		return (SUCCESS);
	return (FAILURE);
}

t_wildcard	*remove_wildcard(t_pipe *info, t_wildcard *prev, t_wildcard *to_remove)
{
	if (prev == to_remove)
		info->wildcard = to_remove->next;
	else
		prev->next = to_remove->next;
	lst_wildcard_free(to_remove);
	return (prev);
}
