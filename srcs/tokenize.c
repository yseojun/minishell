/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 19:14:42 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/15 21:27:58 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

static void		get_token_size(char *str, int *idx);
static int		get_quote_size(char *str, int idx);

int	tokenize(char *str, t_data *data)
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
			new = lst_new_token(str, &idx);
			if (!new || lst_token_add_back(&data->head, new) == FAILURE)
				return (FAILURE);
			if (new->type == 0)
			{
				if (new->left && is_redirection(new->left->token))
					new->type = REDIRECTION;
				else
					new->type = CMD;
			}
		}
	}
	return (SUCCESS);
}

char	*put_token(char *str, int *idx)
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
	int	qoute_size;
	int	start;

	start = *idx;
	while (str[*idx] && str[*idx] != ' ')
	{
		qoute_size = 0;
		if (str[*idx] == '(' || str[*idx] == ')' || is_special_chr(str[*idx]))
		{
			if (start == *idx)
			{
				if (is_special_chr(str[*idx]) && str[*idx + 1] == str[*idx])
					(*idx)++;
				(*idx)++;
			}
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

int	set_type(char *token)
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
