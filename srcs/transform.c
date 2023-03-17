/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 09:01:28 by rolee             #+#    #+#             */
/*   Updated: 2023/03/17 20:20:57 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "parse.h"
#include "util.h"

static int	is_todo_wildcard(t_token *search);
static int	is_surrounded_by_quote(char *token);
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
		if (is_todo_wildcard(search))
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
	if (!data->head)
		return (FAILURE);
	return (SUCCESS);
}

static int	is_todo_wildcard(t_token *search)
{
	if (ft_strchr(search->token, '*') == 0)
		return (FALSE);
	if (is_surrounded_by_quote(search->token))
		return (FALSE);
	if (search->left && ft_strncmp(search->left->token, "<<", 3) == 0)
		return (FALSE);
	return (TRUE);
}

static int	is_surrounded_by_quote(char *token)
{
	int	flag[2];

	flag[0] = 0;
	flag[1] = 0;
	while (*token)
	{
		if (*token == '\'')
		{
			if (flag[0] == 0)
				flag[0]++;
			else
				flag[0]--;
		}
		else if (*token == '\"')
		{
			if (flag[1] == 0)
				flag[1]++;
			else
				flag[1]--;
		}
		else if (*token == '*' && !flag[0] && !flag[1])
			return (FALSE);
		token++;
	}
	return (TRUE);
}

static int	wildcard(t_data *data, t_token **search)
{
	make_wildcard_lst(data, *search);
	if (data->wildcard)
	{
		if (merge_wildcard_lst(data, search) == FAILURE)
			return (FAILURE);
	}
	else
		*search = (*search)->right;
	return (SUCCESS);
}

static void	make_wildcard_lst(t_data *data, t_token *now)
{
	t_wildcard		*to_find;
	DIR				*dp;
	char			*cwd;
	struct dirent	*fp;

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
	to_find = make_to_find_head(now->token);
	if (to_find)
		cmp_wildcard(data, to_find, now);
	lst_wildcard_free_all(to_find);
}
