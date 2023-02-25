/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 18:15:17 by rolee             #+#    #+#             */
/*   Updated: 2023/02/25 18:29:31 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

// transform에서 해줘야 하는 작업

// - $ 환경 변수 확장 처리 ✅
	// $를 만나면 숫자, 알파벳, 언더바인 데까지만 읽어서 value로 교체
	// 작은 따옴표를 대응하는 따옴표 만날 때까지 건너뛰기
	// 큰 따옴표를 만나면 대응하는 따옴표를 만날 때까지 건너뛰면서 $를 만나면 첫 번째처럼 처리

// - 따옴표 제거하기
	// 따옴표를 만나면 제거하고, 다음 대응하는 따옴표를 찾아서 제거한다.

int	get_env_name_len(int env_name_idx, char *str)
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

void	put_in_new_str(char *origin_str, char *new_str, char *env_value, int dollar_idx)
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
	free(origin_str);
}

char	*remove_env_name(char *origin_str, int env_name_len, int dollar_idx)
{
	int		new_size;
	char	*new_str;
	int		origin_idx;
	int		new_idx;

	new_size = ft_strlen(origin_str) - env_name_len - 1;
	new_str = (char *)malloc(new_size + 1);
	if (!new_str)
		exit(1);
	origin_idx = 0;
	new_idx = 0;
	while (origin_str[origin_idx])
	{
		if (origin_idx < dollar_idx || origin_idx > dollar_idx + env_name_len)
			new_str[new_idx++] = origin_str[origin_idx];
		origin_idx++;
	}
	free(origin_str);
	return (new_str);
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
		exit(1);
	env_value = get_env(env_name);
	free(env_name);
	*idx += ft_strlen(env_value);
	if (!env_value)
		return (remove_env_name(origin_str, env_name_len, dollar_idx));
	new_size = ft_strlen(origin_str) - env_name_len + (*idx - dollar_idx);
	new_str = (char *)malloc(new_size + 1);
	if (!new_str)
		exit(1);
	put_in_new_str(origin_str, new_str, env_value, dollar_idx);
	return (new_str);
}

void	handle_single_quotes(char *str, int *idx)
{
	int	new_idx;

	new_idx = *idx + 1;
	while (str[new_idx] != '\'' && str[new_idx])
		new_idx++;
	if (str[new_idx] == '\'')
		*idx = new_idx;
}

char	*handle_double_quotes(char *str, int *idx)
{
	int	new_idx;

	new_idx = *idx + 1;
	while (str[new_idx] != '\"' && str[new_idx])
	{
		if (str[new_idx] == '$')
			str = get_expanded(new_idx, str, &new_idx);
		new_idx++;
	}
	if (str[new_idx] == '\"')
		*idx = new_idx;
	return (str);
}

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


void	transform(char **token_arr)
{
	int	idx;

	idx = 0;
	while (token_arr[idx])
	{
		token_arr[idx] = expand_env(token_arr[idx]);
		//token_arr[idx] = remove_quote(token_arr[idx]);

		printf("token_arr[%d]: %s\n", idx, token_arr[idx]);
		idx++;
	}
}
