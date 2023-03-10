/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_tokenize.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 13:18:45 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/14 12:11:26 by seojyang         ###   ########.fr       */
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

// ?????? ?????? ???, ?????? ?????? ????????? ????????? ??????
// ?????? ?????? ???, ?????? ?????? ????????? ???????????? ?????????????????? ??????

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

	// head ??????
	head = tail;
	while (head->left)
		head = head->left;
	// top ??????
	top = NULL; // ??? ?????? ???????
	search = head;
	while (search)
	{
		if (search->type == CMD)
			break ;
		search = search->right;
	}
	top = search; // CMD??? ???????????? ????????? ??? ?????????????????? top?????? ????????? ???
	//printf("command top: %s\n", top->token);
	// ?????? ?????????
	search = head;
	while (search)
	{
		//printf("search: %s\n", top->token);
		if (search->type == CMD)
			search = add_command(top, search); // top?????? search??? ????????? add ??? ??????
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

	// next_search ??????, ?????? ??????
	next_search = add->right;
	if (next_search)
	{
		next_search->left->right = NULL;
		next_search->left = NULL;
	}	
	// ??????????????? ??? top????????? ?????? next_search ??????
	if (top == add)
		return (next_search);
	// ???????????? add right back
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

	// next_search ??????, ?????? ??????
	next_search = add->right->right;
	if (next_search)
	{
		next_search->left->right = NULL;
		next_search->left = NULL;
	}
	// top??? ????????? add left back, top??? ????????? add??? top?????? ??????
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
