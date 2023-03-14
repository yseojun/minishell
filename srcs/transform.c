/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 09:01:28 by rolee             #+#    #+#             */
/*   Updated: 2023/03/14 18:04:31 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "parse.h"
#include "util.h"

static int	wildcard(t_data *data, t_token **search);
static void	make_wildcard_lst(t_data *data, t_token *now);

int	transform(t_data *data)
{
	t_token	*search;
	int		is_expanded;

	search = data->head;
	while (search)
	{
		is_expanded = 0;
		search->token = expand(data, search->token, &is_expanded);
		if (!is_expanded)
		{
			if (wildcard(data, &search) == FAILURE)
				return (FAILURE);
			continue ;
		}
		search->token = remove_quote(search->token);
		if (is_expanded && ft_strlen(search->token) == 0)
			search = pull_token(&data->head, search);
		else
			search = search->right;
	}
	if (data->head == 0)
		return (FAILURE);
	return (SUCCESS);
}

static int	wildcard(t_data *data, t_token **search)
{
	make_wildcard_lst(data, *search);
	if (data->wildcard)
	{
		if (merge_wildcard_lst(data, search) == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}

static void	make_wildcard_lst(t_data *data, t_token *now)
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
	dp = opendir(cwd);
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
