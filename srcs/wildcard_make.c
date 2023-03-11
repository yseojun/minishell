/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_make.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 12:36:39 by seojun            #+#    #+#             */
/*   Updated: 2023/03/11 13:25:34 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

static void			cmp_wildcard(t_pipe *info, char **to_find);
static int			find_str(char *to_find_str, char *name, int *idx);
static int			check_wildcard(char *name, char **to_find);
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
	while ((fp = readdir(dp)) != NULL)
		wildcard_add_back(&info->wildcard, lst_new_wildcard(fp->d_name));
	to_find = ft_split(now->token, '*');
	cmp_wildcard(info, to_find);
}

static void	cmp_wildcard(t_pipe *info, char **to_find)
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
		if (check_wildcard(search->name, to_find) == FAILURE)
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
	if (to_find[to_find_idx] == 0 && name[idx] == 0) // 추가
		return (SUCCESS);
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
