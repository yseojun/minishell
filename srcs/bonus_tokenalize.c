/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_tokenalize.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 13:18:45 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/04 18:46:03 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "parse.h"

int	bonus_tokenalize(char *str, t_pipe *info)
{
	t_token	*head;
	t_token	*new;
	char	**new_arr;
	int		idx;
	int		n;

	head = (t_token *)malloc(sizeof(t_token));
	head->left = 0;
	head->right = 0;
	head->token = 0;
	idx = 0;
	while (str[idx])
	{
		if (str[idx] == ' ')
			idx++;
		else
		{
			if (put_token(str, new_arr, n, &idx) == FAILURE)
				return (FAILURE);
			n++;
		}
	}
	return (SUCCESS);
}





t_token *make_tree(t_token *tail)
{
	t_token	*search;
	t_token	*save;

	save = tail;
	search = tail;
	while (search)
	{
		if (타입이 괄호)
		{
			닫는 괄호를 삭제하고 끊어야 됨
			여는 괄호가 올때까지 찾아서 삭제하고 끊고
			return (make_tree(tail(search)));
		}
		else if (타입이 논리면)
		{
			search->left = make_tree(search->left);
			search->right = make_tree(tail(search));
			cut();
			retrun(search);
		}
		else if (타입이 파이프)
		{
			search->left = make_tree(search->left);
			search->right = make_tree(tail(search));
			cut();
			retrun(search);
		}
		else if (타입이 커맨드)
		{
			커맨드 앞 뒤로 끊고
			return (커맨드 맨 처음 노드)
		}
		else if (타입이 리다이렉션)
		{
			리다이렉션 앞 뒤로 끊고
			return (리다이렉션 기호 노드)
		}
		search = search->left;
	}
}

t_token *make_tree(t_token *tail)
{
	if (find_logic())
		return (논리);
	if (괄호를 찾으면)
		return (괄호안의 최상단 노드);
	if (파이프를 찾으면)
		return (파이프 노드)
	if (커맨드)
		return (커맨드 노드)
	if (리다이렉션)
		return ()
	return (0);
}

t_token *find_logic()
{
	while ()
	{
		if (타입이 논리면)
		{
			search->left = make_tree(search->left);
			search->right = make_tree(tail(search));
			cut();
			retrun(search);
		}
	}
	return (0);
}

t_token *find_braces()
{
	while()
	{
		if (타입이 괄호)
		{
			닫는 괄호를 삭제하고 끊어야 됨
			여는 괄호가 올때까지 찾아서 삭제하고 끊고
			return (make_tree(tail(search)));
		}
	}
	return (0);
}

t_token *find_pipe()
{
	while()
	{
		if (타입이 괄호)
		{
			닫는 괄호를 삭제하고 끊어야 됨
			여는 괄호가 올때까지 찾아서 삭제하고 끊고
			return (make_tree(tail(search)));
		}
	}
	return (0);
}

t_token *find_braces(t_token *tail)
{
	while()
	{
		if ()
		{
			닫는 괄호를 삭제하고 끊어야 됨
			여는 괄호가 올때까지 찾아서 삭제하고 끊고
			return (make_tree(tail(search)));
		}
	}
	return (0);
}
