/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 09:01:28 by rolee             #+#    #+#             */
/*   Updated: 2023/03/07 20:19:51 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "parse.h"

#include "util.h"

// cd 명령에서 expand해줘야 할 게 있을 수도(~, -)
// bonus에서 * 와일드카드

int	wildcard(t_pipe *info, t_token *now)
{
	char			**to_find;
	int				idx;
	int				find_idx;
	
	if (ft_strncmp("<<", now->left->token, 3) == 0)
		return (SUCCESS);
	if (!ft_strchr(now->token, '*'))
		return (SUCCESS);
	to_find = ft_split(now->token, '*');
	idx = 0;
	find_idx = 0;
	info->wildcard = make_wildcard(info);
	cmp_wildcard(info, now);
	if (attach_token(info, now) == FAILURE)
		return (FAILURE);
}

t_wildcard *make_wildcard(t_pipe *info)
{
	DIR				*dp;
	struct dirent	*fp;
	dp = opendir(getcwd(0, 0));
	
}

int	cmp_wildcard()
{
	
}

int	transform(t_data *data, t_pipe *info)
{
	t_token	*search;
	int		is_expanded;

	search = info->head;
	while (search)
	{
		is_expanded = 0;
		search->token = expand(data, search->token, &is_expanded);
		if (is_expanded)
		{
			if (wildcard(info) == FAILURE)
				return (FAILURE);
		}
		search->token = remove_quote(search->token);
		if (is_expanded && ft_strlen(search->token) == 0)
			search = pull_token(&info->head, search);
		else
			search = search->right;
	}
	return (SUCCESS);
}



// 토큰에 *이 있으면
	// 해당 토큰에 있는 별 이외의 문자를 연결리스트로 저장
	// readdir 하면서 읽어온 이름에서 

// void	transform(t_data *data, t_pipe *info)
// {
// 	int	idx;
// 	int	is_expanded;

// 	idx = 0;
// 	while (info->token_arr[idx])
// 	{
// 		is_expanded = 0;
// 		info->token_arr[idx] = expand(data, info->token_arr[idx], &is_expanded);
// 		info->token_arr[idx] = remove_quote(info->token_arr[idx]);
// 		if (is_expanded && ft_strlen(info->token_arr[idx]) == 0)
// 		{
// 			pull_token(info->token_arr, idx);
// 			info->token_arr_size--;
// 		}
// 		idx++;
// 	}
// }
