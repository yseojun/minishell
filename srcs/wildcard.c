/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 21:56:53 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/18 20:56:46 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "parse.h"

static void	make_wildcard_lst(t_data *data, t_token *now);

int	wildcard(t_data *data, t_token **search)
{
	make_wildcard_lst(data, *search);
	if (data->wildcard)
	{
		if (merge_wildcard_lst(data, search) == FAILURE)
			return (FAILURE);
	}
	else
	{
		(*search)->token = remove_quote((*search)->token);
		*search = (*search)->right;
	}
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
	if (!dp)
		exit(EXIT_FAILURE);
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
