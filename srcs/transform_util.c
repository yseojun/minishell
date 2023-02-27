/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_util.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 09:07:30 by rolee             #+#    #+#             */
/*   Updated: 2023/02/27 16:12:32 by rolee            ###   ########.fr       */
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
	*idx = new_idx - 1;
	return (str);
}

void	pull_token(char **token_arr, int idx)
{
	free(token_arr[idx]);
	while (token_arr[idx])
	{
		if (token_arr[idx + 1] == 0)
			break ;
		token_arr[idx] = token_arr[idx + 1];
		idx++;
	}
	token_arr[idx] = 0;
}
