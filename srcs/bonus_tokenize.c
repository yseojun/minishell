/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_tokenize.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 13:18:45 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/13 20:49:09 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "parse.h"

static t_token	*find_logical_operator(t_token *tail);
static t_token	*find_pipe(t_token *tail);
static t_token	*find_braces(t_token *tail);
static t_token	*find_command(t_token *tail);
static t_token	*add_command(t_token *top, t_token *add);
static t_token	*add_redirection(t_token **top, t_token *add);

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

static t_token	*find_logical_operator(t_token *tail)
{
	t_token	*search;

	search = tail;
	while (search)
	{
		if (search->type == BRACE)
		{
			search = search->left;
			while (search->type != BRACE)
				search = search->left;
		}
		else if (search->type == AND || search->type == OR)
		{
			if (search->left)
			{
				search->left->right = NULL;
				search->left = make_tree(search->left);	
			}
			if (search->right)
			{
				search->right->left = NULL;
				search->right = make_tree(tail);
			}
			return(search);
		}
		search = search->left;
	}
	return (0);
}

static t_token *find_pipe(t_token *tail)
{
	t_token	*search;

	search = tail;
	while(search)
	{
		if (search->type == BRACE)
		{
			search = search->left;
			while (search->type != BRACE)
				search = search->left;
		}
		else if (search->type == PIPE)
		{
			if (search->left)
			{
				search->left->right = NULL;
				search->left = make_tree(search->left);
			}
			if (search->right)
			{
				search->right->left = NULL;
				search->right = make_tree(tail);
			}
			return (search);
		}
		search = search->left;
	}
	return (0);
}

// 여는 괄호 앞, 닫는 괄호 뒤에는 기호만 가능
// 여는 괄호 뒤, 닫는 괄호 앞에는 커맨드와 리다이렉션만 가능

static t_token *find_braces(t_token *tail)
{
	t_token	*search;
	t_token	*to_remove;

	search = tail;
	while(search)
	{
		if (search->type == BRACE)
		{
			search->left->right = search->right;
			if (search->right)
				search->right->left = search->left;
			to_remove = search;
			search = search->left;
			lst_token_free(to_remove);
			to_remove = NULL;
			while (search->left)
				search = search->left;
			search = search->right;
			lst_token_free(search->left);
			search->left = NULL;
			return (make_tree(lst_token_last(search)));
		}
		search = search->left;
	}
	return (0);
}

static t_token	*find_command(t_token *tail)
{
	t_token	*head;
	t_token	*search;
	t_token	*top;

	// head 찾기
	head = tail;
	while (head->left)
		head = head->left;
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
	//printf("command top: %s\n", top->token);
	// 트리 만들기
	search = head;
	while (search)
	{
		//printf("search: %s\n", top->token);
		if (search->type == CMD)
			search = add_command(top, search); // top이랑 search가 같으면 add 안 하기
		if (search && search->type == REDIRECTION)
			search = add_redirection(&top, search);
	}
	//printf("left : %p, right : %p\n", top->left, top->right);
	return (top);
}

static t_token	*add_command(t_token *top, t_token *add)
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
		return (next_search);
	// 아니라면 add right back
	search = top;
	while (search->right)
		search = search->right;
	search->right = add;
	add->left = NULL;
	return (next_search);
}

static t_token	*add_redirection(t_token **top, t_token *add)
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
	if (*top)
	{
		search = *top;
		while (search->left)
			search = search->left;
		search->left = add;
	}
	else
		*top = add;
	add->left = NULL;
	add->right->left = 0;
	return (next_search);
}
