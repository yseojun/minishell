/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 13:50:59 by rolee             #+#    #+#             */
/*   Updated: 2023/02/26 14:24:51 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

static int	get_env_name_len(int env_name_idx, char *str);
static char	*remove_env_name(char *origin_str, int env_name_len, int dollar_idx);
static void	put_in_new_str(char *origin_str, char *new_str, char *env_value, int dollar_idx);

char	*expand_env(char *str)
{
	int	idx;
	
	idx = 0;
	while (str[idx])
	{
		if (str[idx] == '$')
			str = get_expanded(idx, str, &idx);
		if (str[idx] == '\'')
			handle_single_quotes(str, &idx);
		if (str[idx] == '\"')
			str = handle_double_quotes(str, &idx);
		idx++;
	}
	return (str);
}

char	*get_expanded(int dollar_idx, char *origin_str, int *idx)
{
	int		env_name_len;
	char	*env_name;
	char	*env_value;
	int		new_size;
	char	*new_str;

	env_name_len = get_env_name_len(dollar_idx + 1, origin_str);
	env_name = ft_substr(origin_str, dollar_idx + 1, env_name_len);
	if (!env_name)
		exit(EXIT_FAILURE);
	env_value = get_env(env_name);
	free(env_name);
	*idx += ft_strlen(env_value);
	if (!env_value)
		return (remove_env_name(origin_str, env_name_len, dollar_idx));
	new_size = ft_strlen(origin_str) - env_name_len + (*idx - dollar_idx);
	new_str = (char *)malloc(new_size + 1);
	if (!new_str)
		exit(EXIT_FAILURE);
	put_in_new_str(origin_str, new_str, env_value, dollar_idx);
	return (new_str);
}

static int	get_env_name_len(int env_name_idx, char *str)
{
	int	env_name_len;

	env_name_len = 0;
	while (ft_isalnum(str[env_name_idx]) || str[env_name_idx] == '_')
	{
		env_name_len++;
		env_name_idx++;
	}
	return (env_name_len);
}

static char	*remove_env_name(char *origin_str, int env_name_len, int dollar_idx)
{
	int		new_size;
	char	*new_str;
	int		origin_idx;
	int		new_idx;

	new_size = ft_strlen(origin_str) - env_name_len - 1;
	new_str = (char *)malloc(new_size + 1);
	if (!new_str)
		exit(EXIT_FAILURE);
	origin_idx = 0;
	new_idx = 0;
	while (origin_str[origin_idx])
	{
		if (origin_idx < dollar_idx || origin_idx > dollar_idx + env_name_len)
			new_str[new_idx++] = origin_str[origin_idx];
		origin_idx++;
	}
	new_str[new_idx] = 0;
	free(origin_str);
	return (new_str);
}

static void	put_in_new_str(char *origin_str, char *new_str, char *env_value, int dollar_idx)
{
	int	origin_idx;
	int	new_idx;
	int	env_idx;

	origin_idx = 0;
	new_idx = 0;
	env_idx = 0;
	while (origin_str[origin_idx])
	{
		if (origin_idx == dollar_idx)
		{
			while (env_value[env_idx])
				new_str[new_idx++] = env_value[env_idx++];
			origin_idx += get_env_name_len(origin_idx + 1, origin_str) + 1;
			if (!origin_str[origin_idx])
				break ;
		}
		new_str[new_idx++] = origin_str[origin_idx++];
	}
	new_str[new_idx] = 0;
	free(origin_str);
}
