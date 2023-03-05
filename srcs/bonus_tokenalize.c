/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_tokenalize.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 13:18:45 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/05 15:06:49 by rolee            ###   ########.fr       */
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


// t_token *make_tree(t_token *tail)
// {
// 	t_token	*search;
// 	t_token	*save;

// 	save = tail;
// 	search = tail;
// 	while (search)
// 	{
// 		if (타입이 괄호)
// 		{
// 			닫는 괄호를 삭제하고 끊어야 됨
// 			여는 괄호가 올때까지 찾아서 삭제하고 끊고
// 			return (make_tree(tail(search)));
// 		}
// 		else if (타입이 논리면)
// 		{
// 			search->left = make_tree(search->left);
// 			search->right = make_tree(tail(search));
// 			cut();
// 			retrun(search);
// 		}
// 		else if (타입이 파이프)
// 		{
// 			search->left = make_tree(search->left);
// 			search->right = make_tree(tail(search));
// 			cut();
// 			retrun(search);
// 		}
// 		else if (타입이 커맨드)
// 		{
// 			커맨드 앞 뒤로 끊고
// 			return (커맨드 맨 처음 노드)
// 		}
// 		else if (타입이 리다이렉션)
// 		{
// 			리다이렉션 앞 뒤로 끊고
// 			return (리다이렉션 기호 노드)
// 		}
// 		search = search->left;
// 	}
// }

t_token *make_tree(t_token *tail)
{
	t_token	*top;

	top = find_logical_operator(tail);
	if (top)
		return (top);
	top = find_pipe(tail);
	if (top)
		return (top);
	top = find_braces(tail);
	if (top)
		return (top);
	top = find_command(tail);
	if (top)
		return (top);
	return (0);
}

t_token *find_logical_operator(t_token *tail)
{
	t_token	*search;

	search = tail;
	while (search)
	{
		if (search->type == AND || search->type == OR)
		{
			search->right->left = NULL;
			search->left->right = NULL;
			search->left = make_tree(search->left);
			search->right = make_tree(tail);
			retrun(search);
		}
		search = search->left;
	}
	return (0);
}

t_token *find_pipe(t_token *tail)
{
	t_token	*search;

	search = tail;
	while(search)
	{
		if (search->type == PIPE)
		{
			search->right->left = NULL;
			search->left->right = NULL;
			search->left = make_tree(search->left);
			search->right = make_tree(tail);
			return (search);
		}
		search = search->left;
	}
	return (0);
}

// 여는 괄호 앞, 닫는 괄호 뒤에는 기호만 가능
// 여는 괄호 뒤, 닫는 괄호 앞에는 커맨드와 리다이렉션만 가능

t_token *find_braces(t_token *tail)
{
	t_token	*search;
	t_token	*to_remove;

	search = tail;
	while(search)
	{
		if (search->type = BRACE)
		{
			search->left->right = search->right;
			if (search->right)
				search->right->left = search->left;
			to_remove = search;
			search = search->left;
			free(to_remove->token);
			free(to_remove);
			to_remove = NULL;
			while (search->left)
				search = search->left;
			search = search->right;
			free(search->left->token);
			free_(search->left);
			search->left = NULL;
			return (make_tree(lst_last_token(search)));
		}
		search = search->left;
	}
	return (0);
}

t_token	*find_command(t_token *tail)
{
	t_token	*head;
	t_token	*search;
	t_token	*top;
	int		count;

	// head 찾기
	head = tail;
	while (head->left)
		head = head->left
	// top 설정
	top = NULL; // 안 해도 되나?
	search = head;
	while (search)
	{
		if (search->type == CMD)
			break ;
		search = search->right;
	}
	top = search; // CMD가 없었다면 나중에 첫 리다이렉션이 top으로 설정될 것
	// 트리 만들기
	search = head;
	while (search)
	{
		if (search->type == CMD)
			search = add_command(top, search); // top이랑 search가 같으면 add 안 하기
		if (search && search->type == REDIRECTION)
			search == add_redirection(top, search);
	}
	return (head);
}

t_token	*add_command(t_token *top, t_token *add)
{
	t_token	*next_search;
	t_token	*search;

	// next_search 설정, 연결 끊기
	next_search = add->right;
	if (next_search)
	{
		next_search->left->right = NULL;
		next_search->left = NULL;
	}	
	// 추가하려는 게 top이라면 바로 next_search 리턴
	if (top == add)
		return (add->right);
	// 아니라면 add right back
	search = top;
	while (search->right)
		search = search->right;
	search->right = add;
	return (next_search);
}

t_token	*add_redirection(t_token *top, t_token *add)
{
	t_token	*next_search;
	t_token	*search;

	// next_search 설정, 연결 끊기
	next_search = add->right->right;
	if (next_search)
	{
		next_search->left->right = NULL;
		next_search->left = NULL;
	}
	// top이 있다면 add left back, top이 없다면 add를 top으로 설정
	if (top)
	{
		search = top;
		while (search->left)
			search = search->left;
		search->left = add;
	}
	else
		top = add;
	add->left = NULL;
	return (next_search);
}
