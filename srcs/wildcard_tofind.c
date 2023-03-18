/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_tofind.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 18:35:35 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/18 15:40:17 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "parse.h"

static char	*put_tofind(char *str, int *idx);

static t_wildcard	*lst_new_tofind(char *str, int *idx)
{
	t_wildcard	*new;

	new = (t_wildcard *)malloc(sizeof(t_wildcard));
	if (!new)
		exit(EXIT_FAILURE);
	new->name = put_tofind(str, idx);
	new->name = remove_quote(new->name);
	new->next = NULL;
	return (new);
}

static char	*put_tofind(char *str, int *idx)
{
	int		to_find_len;
	int		quote_size;
	char	*to_find;

	to_find_len = 0;
	while (str[*idx] && str[*idx] != '*')
	{
		quote_size = 0;
		if (str[*idx] == '\'' || str[*idx] == '\"')
		{
			quote_size = get_quote_size(str, *idx);
			(*idx) += quote_size;
			to_find_len += quote_size;
		}
		to_find_len++;
		(*idx)++;
	}
	to_find = (char *)malloc(sizeof(char) * (to_find_len + 1));
	if (!to_find)
		exit(EXIT_FAILURE);
	ft_memcpy(to_find, str + (*idx - to_find_len), to_find_len);
	to_find[to_find_len] = 0;
	return (to_find);
}

t_wildcard	*make_to_find_head(char *s)
{
	t_wildcard	*to_find_head;
	int			idx;

	idx = 0;
	to_find_head = 0;
	while (s[idx])
	{
		while (s[idx] && s[idx] == '*')
			idx++;
		wildcard_add_back(&to_find_head, lst_new_tofind(s, &idx));
	}
	return (to_find_head);
}
