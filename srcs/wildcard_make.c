/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_make.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 12:36:39 by seojun            #+#    #+#             */
/*   Updated: 2023/03/11 15:46:01 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

static void			cmp_wildcard(t_pipe *info, char **to_find, t_token *now);
static int			find_str(char *to_find_str, char *name, int *idx);
static int			check_wildcard(char *name, char **to_find, t_token *now);
static t_wildcard	*remove_wildcard(t_pipe *info, t_wildcard *prev, \
	t_wildcard *to_remove);

void	make_wildcard_lst(t_pipe *info, t_token *now)
{
	char			**to_find;
	DIR				*dp;
	struct dirent	*fp;

	if (now->left && ft_strncmp("<<", now->left->token, 3) == 0)
		return ;
	if (!ft_strchr(now->token, '*'))
		return ;
	dp = opendir(getcwd(0, 0)); // 실패할 경우 처리
	while (1)
	{
		fp = readdir(dp);
		if (fp == NULL)
			break ;
		wildcard_add_back(&info->wildcard, lst_new_wildcard(fp->d_name));
	}
	to_find = ft_split(now->token, '*');
	cmp_wildcard(info, to_find, now);
}

static void	cmp_wildcard(t_pipe *info, char **to_find, t_token *now)
{
	t_wildcard	*search;
	t_wildcard	*prev;

	if (info->wildcard == 0)
		return ;
	search = info->wildcard;
	prev = search;
	wildcard_prt(info->wildcard);
	while (search)
	{
		printf("search name : %s\n", search->name);
		if (check_wildcard(search->name, to_find, now) == FAILURE)
			search = remove_wildcard(info, prev, search);
		else
		{
			prev = search;
			search = search->next;
		}
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
	if (token[ft_strlen(token)] != '*')
	{
		find_len = ft_strlen(*to_find);
		if (name_len < find_len
			|| ft_strncmp(name + name_len - find_len, *to_find, find_len) != 0)
			return (0);
	}
	return (1);
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

static t_wildcard	*remove_wildcard(t_pipe *info, t_wildcard *prev, \
	t_wildcard *to_remove)
{
	if (to_remove == info->wildcard)
	{
		info->wildcard = to_remove->next;
		lst_wildcard_free(to_remove);
		return (info->wildcard);
	}
	else
	{
		prev->next = to_remove->next;
		lst_wildcard_free(to_remove);
		return (prev->next);
	}
}
