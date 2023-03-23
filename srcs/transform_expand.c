/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_expand.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 09:40:41 by rolee             #+#    #+#             */
/*   Updated: 2023/03/23 12:57:21 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "parse.h"

static int	get_key_len(int key_idx, char *str);
static void	put_in_new_str(char *str, char *new, char *value, int dollar);

char	*expand(t_data *data, char *str, int *flag)
{
	int	idx;

	idx = 0;
	while (str[idx])
	{
		if (str[idx] == '$')
		{
			str = get_expanded(data, idx, str, &idx);
			*flag = 1;
		}
		else if (str[idx] == '\'')
			handle_single_quote(str, &idx);
		else if (str[idx] == '\"')
			str = handle_double_quote(data, str, &idx);
		idx++;
	}
	return (str);
}

char	*get_expanded(t_data *data, int dollar, char *str, int *idx)
{
	int			key_len;
	char		*key;
	char		*value;
	int			new_size;
	char		*new_str;

	key_len = get_key_len(dollar + 1, str);
	if (key_len == 0)
		return (str);
	key = ft_substr(str, dollar + 1, key_len);
	if (!key)
		exit(EXIT_FAILURE);
	value = get_value(data, key);
	if (!value)
		value = ft_strdup("");
	free(key);
	*idx += ft_strlen(value) - 1;
	new_size = ft_strlen(str) - key_len + ft_strlen(value) - 1;
	new_str = (char *)malloc(new_size + 1);
	if (!new_str)
		exit(EXIT_FAILURE);
	put_in_new_str(str, new_str, value, dollar);
	free(value);
	return (new_str);
}

static int	get_key_len(int key_idx, char *str)
{
	int	key_len;

	key_len = 0;
	if (str[key_idx] == '?')
		return (1);
	while (ft_isalnum(str[key_idx]) || str[key_idx] == '_')
	{
		key_len++;
		key_idx++;
	}
	return (key_len);
}

char	*get_value(t_data *data, char *key)
{
	t_env	*search;

	if (ft_strncmp(key, "?", 2) == 0)
		return (ft_itoa(exit_status(LOAD)));
	search = data->env;
	while (search)
	{
		if (ft_strncmp(search->name, key, ft_strlen(key) + 1) == 0)
			return (ft_strdup(search->value));
		search = search->next;
	}
	return (NULL);
}

static void	put_in_new_str(char *str, char *new, char *value, int dollar)
{
	int	idx;
	int	new_idx;
	int	value_idx;

	idx = 0;
	new_idx = 0;
	value_idx = 0;
	while (str[idx])
	{
		if (idx == dollar)
		{
			while (value[value_idx])
				new[new_idx++] = value[value_idx++];
			idx += get_key_len(idx + 1, str) + 1;
			if (!str[idx])
				break ;
		}
		new[new_idx++] = str[idx++];
	}
	new[new_idx] = 0;
	free(str);
}
