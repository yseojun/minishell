/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 09:01:28 by rolee             #+#    #+#             */
/*   Updated: 2023/03/27 19:52:54 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "parse.h"
#include "util.h"

static int		is_todo_wildcard(char *token);

void	transform(t_data *data, t_token *top)
{
	int		is_expanded;

	if (!top)
		return ;
	transform(data, top->left);
	if (ft_strncmp(top->token, "<<", 3) != 0)
		transform(data, top->right);
	is_expanded = 0;
	top->token = expand(data, top->token, &is_expanded);
	if (is_todo_wildcard(top->token))
		wildcard(data, &top);
	else
	{
		top->token = remove_quote(top->token);
		if (is_expanded && ft_strlen(top->token) == 0)
			top->type = ERROR;
	}
}

static int	is_todo_wildcard(char *token)
{
	int	single_flag;
	int	double_flag;

	single_flag = 0;
	double_flag = 0;
	while (*token)
	{
		if (*token == '\'')
			single_flag = single_flag != 1;
		else if (*token == '\"')
			double_flag = double_flag != 1;
		else if (*token == '*' && !single_flag && !double_flag)
			return (TRUE);
		token++;
	}
	return (FALSE);
}
