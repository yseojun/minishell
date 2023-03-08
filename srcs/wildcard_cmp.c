/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_cmp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojun <seojun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 12:36:39 by seojun            #+#    #+#             */
/*   Updated: 2023/03/08 12:51:46 by seojun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

int	find_str(char *to_find_str, char *name, int *idx)
{
	while (name[*idx])
	{
		if (ft_strncmp(name + *idx, to_find_str, ft_strlen(to_find_str)))
			return (SUCCESS);
		(*idx)++;
	}
	return (FAILURE);
}

int	check_wildcard(char *name, char **to_find)
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
		return(SUCCESS);
	return (FAILURE);
}

int	cmp_wildcard(t_pipe *info, t_token *now, char **to_find)
{
	t_wildcard	*search;
	
	if (info->wildcard == 0)
		return (SUCCESS);
	search = info->wildcard;
	while (search)
	{
		if (check_wildcard(search->name, to_find) == -1)
			remove_wildcard(info, search);
		search = search->next;
	}
}
