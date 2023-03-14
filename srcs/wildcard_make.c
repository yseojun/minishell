/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_make.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 12:36:39 by seojun            #+#    #+#             */
/*   Updated: 2023/03/14 15:20:16 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"

static void			cmp_wildcard(t_data *data, char **to_find, t_token *now);
static int			find_str(char *to_find_str, char *name, int *idx);
static int			check_wildcard(char *name, char **to_find, t_token *now);
static t_wildcard	*remove_wildcard(t_data *data, t_wildcard *prev, \
	t_wildcard *to_remove);

void	make_wildcard_lst(t_data *data, t_token *now)
{
	char			**to_find;
	DIR				*dp;
	char			*cwd;
	struct dirent	*fp;

	if (now->left && ft_strncmp("<<", now->left->token, 3) == 0)
		return ;
	if (!ft_strchr(now->token, '*'))
		return ;
	cwd = getcwd(0, 0);
	dp = opendir(cwd); // 실패할 경우 처리
	free(cwd);
	while (1)
	{
		fp = readdir(dp);
		if (fp == NULL)
			break ;
		if (fp->d_name[0] != '.')
			wildcard_add_back(&data->wildcard, lst_new_wildcard(fp->d_name));
	}
	closedir(dp);
	to_find = ft_split(now->token, '*');
	if (to_find)
		cmp_wildcard(data, to_find, now);
	free_arr((void **)to_find);
}

static void	cmp_wildcard(t_data *data, char **to_find, t_token *now)
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
	if (token[ft_strlen(token) - 1] != '*')
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
