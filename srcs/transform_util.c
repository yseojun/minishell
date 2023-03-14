/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_util.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 09:07:30 by rolee             #+#    #+#             */
/*   Updated: 2023/03/14 09:13:43 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "parse.h"

void	handle_single_quote(char *str, int *idx)
{
	int	new_idx;

	new_idx = *idx + 1;
	while (str[new_idx] != '\'' && str[new_idx])
		new_idx++;
	if (str[new_idx] == '\'')
		*idx = new_idx;
}

char	*handle_double_quote(t_data *data, char *str, int *idx)
{
	int	new_idx;

	new_idx = *idx + 1;
	while (str[new_idx] != '\"' && str[new_idx])
	{
		if (str[new_idx] == '$')
			str = get_expanded(data, new_idx, str, &new_idx);
		new_idx++;
	}
	*idx = new_idx;
	return (str);
}

t_token	*pull_token(t_token **head, t_token *remove)
{
	t_token	*temp;

	temp = remove->right;
	if (remove->left)
		remove->left->right = remove->right;
	if (remove->right)
	{
		if (*head == remove)
			*head = remove->right;
		remove->right->left = remove->left;
	}
	if (!remove->left && !remove->right)
		*head = 0;
	lst_token_free(remove);
	remove = NULL;
	return (temp);
}
