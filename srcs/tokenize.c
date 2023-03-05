/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 19:14:42 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/05 11:53:31 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

static int		get_quote_size(char *str, int idx);
static t_token	*make_token(char *str, int *idx);
static char		*put_token(char *str, int *idx);
static void		get_token_size(char *str, int *idx);
static int		set_type(char *token);
// static size_t	get_arr_size(char *str);
// static int		put_token(char *str, char **new, int n, int *idx);

int	tokenize(char *str, t_pipe *info)
{
	t_token	*new;
	int		idx;

	idx = 0;
	while (str[idx])
	{
		if (str[idx] == ' ')
			idx++;
		else
		{
			new = make_token(str, &idx);
			if (!new || token_add_back(&info->head, new) == FAILURE)
				return (FAILURE);
			if (new->type == 0)
			{
				if (new->left && (new->left->type == CMD
						|| new->left->type == REDIRECTION))
						new->type = new->left->type;
				else
					new->type = CMD;
			}
		}
	}
	// token_prt(info->head);
	return (SUCCESS);
}

static t_token	*make_token(char *str, int *idx)
{
	t_token	*new;

	new = (t_token *)malloc(sizeof(t_token));
	new->left = 0;
	new->right = 0;
	new->token = put_token(str, idx);
	new->type = set_type(new->token);
	return (new);
}

static int	set_type(char *token)
{
	if (is_pipe(token))
		return (PIPE);
	else if (is_redirection(token))
		return (REDIRECTION);
	else if (is_brace(token))
		return (BRACE);
	else if (!ft_strncmp(token, "&&", 3))
		return (AND);
	else if (!ft_strncmp(token, "||", 3))
		return (OR);
	return (0);
}

static char	*put_token(char *str, int *idx)
{
	int		start;
	char	*new;
	int		token_size;

	start = *idx;
	get_token_size(str, idx);
	token_size = *idx - start;
	new = (char *)malloc(token_size + 1);
	if (!new)
		return (0);
	ft_strlcpy(new, str + start, token_size + 1);
	return (new);
}

static void	get_token_size(char *str, int *idx)
{
	int		qoute_size;

	while (str[*idx] && str[*idx] != ' ')
	{
		qoute_size = 0;
		if (is_special_chr(str[*idx]))
		{
			if (str[*idx + 1] == str[*idx])
				(*idx)++;
			(*idx)++;
			break ;
		}
		else if (str[*idx] == '\'' || str[*idx] == '\"')
		{
			qoute_size = get_quote_size(str, *idx);
			*idx += qoute_size;
		}
		(*idx)++;
	}
}

static int	get_quote_size(char *str, int idx)
{
	int		quote_size;
	char	quote;

	quote_size = 0;
	quote = str[idx];
	idx++;
	quote_size++;
	while (str[idx] && str[idx] != quote)
	{
		idx++;
		quote_size++;
	}
	if (!str[idx])
		return (0);
	return (quote_size);
}

// int	tokenize(char *str, t_pipe *info)
// {
// 	char	**new_arr;
// 	int		idx;
// 	int		n;

// 	new_arr = (char **)malloc(sizeof(char *) * get_arr_size(str));
// 	if (!new_arr)
// 		return (FAILURE);
// 	n = 0;
// 	idx = 0;
// 	while (str[idx])
// 	{
// 		if (str[idx] == ' ')
// 			idx++;
// 		else
// 		{
// 			if (put_token(str, new_arr, n, &idx) == FAILURE)
// 				return (FAILURE);
// 			n++;
// 		}
// 	}
// 	new_arr[n] = 0;
// 	info->token_arr = new_arr;
// 	info->token_arr_size = n;
// 	return (SUCCESS);
// }


// static size_t	get_arr_size(char *str)
// {
// 	size_t	count;
// 	int		idx;

// 	count = 0;
// 	idx = 0;
// 	while (str[idx])
// 	{
// 		if (str[idx] == ' ')
// 		{
// 			while (str[idx] == ' ')
// 				idx++;
// 		}
// 		else
// 		{
// 			while (str[idx] && str[idx] != ' ')
// 			{
// 				if (is_redir_pipr_chr(str[idx]))
// 				{
// 					if (str[idx + 1] == str[idx])
// 						idx++;
// 					idx++;
// 					break ;
// 				}
// 				else if (str[idx] == '\'' || str[idx] == '\"')
// 					idx += get_quote_size(str, idx);
// 				idx++;
// 			}
// 			count++;
// 		}
// 	}
// 	return (count + 1);
// }

// static int	put_token(char *str, char **new, int n, int *idx)
// {
// 	int		i;
// 	int		start;
// 	int		token_size;
// 	int		qoute_size;

// 	token_size = 0;
// 	start = *idx;
// 	while (str[*idx] && str[*idx] != ' ')
// 	{
// 		qoute_size = 0;
// 		if (is_redir_pipr_chr(str[*idx]))
// 		{
// 			if (str[*idx + 1] == str[*idx])
// 			{
// 				token_size++;
// 				(*idx)++;
// 			}
// 			(*idx)++;
// 			token_size++;
// 			break ;
// 		}
// 		else if (str[*idx] == '\'' || str[*idx] == '\"')
// 		{
// 			qoute_size = get_quote_size(str, *idx);
// 			*idx += qoute_size;
// 			token_size += qoute_size;
// 		}
// 		(*idx)++;
// 		token_size++;
// 	}
// 	new[n] = (char *)malloc(token_size + 1);
// 	if (!new[n])
// 		return (FAILURE);
// 	i = 0;
// 	while (i < token_size)
// 	{
// 		new[n][i] = str[start + i];
// 		i++;
// 	}
// 	new[n][i] = '\0';
// 	return (SUCCESS);
// }
