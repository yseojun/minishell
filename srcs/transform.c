/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 18:15:17 by rolee             #+#    #+#             */
/*   Updated: 2023/02/20 14:06:50 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "pipe.h"

char	*get_env(char *str);

// transform에서 해줘야 하는 작업
// - $ 환경 변수 확장 처리하기
	// $ 를 만나면 특수문자가 나오기 전까지 읽어서 get_env (해당 환경 변수가 없다면 아무것도 하지 않음)
	// 작은 따옴표를 만나면 대응하는 따옴표를 만날 때까지 인덱스를 건너뛴다.
	// 큰 따옴표를 만나면 대응하는 따옴표를 만날 때까지 인덱스를 건너뛰면서 $ 를 만나면 첫 번째처럼 처리한다.
	
// - 따옴표 제거하기
	// 따옴표를 만나면 제거하고, 다음 대응하는 따옴표를 찾아서 제거한다.

int	get_env_name_len(int env_name_idx, char *origin_str)
{
	int	env_name_len;

	env_name_len = 0;
	while (origin_str[env_name_idx++] > 47)
		env_name_len++;	
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

char	*get_expanded(int env_name_start, char *origin_str, int *env_value_len)
{
	int		env_name_len;
	char	*env_name;
	char	*env_value;
	int		new_size;
	char	*new_str;

	// 환경변수 value 구하기
	env_name_len = get_env_name_len(env_name_start, origin_str);
	env_name = ft_substr(origin_str, env_name_start, env_name_len);
	if (!env_name)
		exit(1);
	env_value = get_env(env_name);
	*env_value_len = ft_strlen(env_value);
	if (!env_value) // env_name에 해당하는 value가 없으면 $(env_name) 부분 제거
		return (remove_env_name(origin_str, env_name_len, env_name_start - 1));
	free(env_name);

	// 알맞은 길이 구해서 재할당
	new_size = ft_strlen(origin_str) - env_name_len + *env_value_len;
	new_str = (char *)malloc(new_size + 1);
	if (!new_str)
		exit(1);

	// new_str에 데이터 넣기
	put_in_new_str(origin_str, new_str, env_value, env_name_start - 1);
	
	return (new_str);
}

char	*expand_env(char *origin_str)
{
	int		idx;
	int		env_value_len;
	
	idx = 0;
	while (origin_str[idx])
	{
		if (origin_str[idx] == '$')
		{
			origin_str = get_expanded(idx + 1, origin_str, &env_value_len);
			idx += env_value_len;
		}
		idx++;
	}
	return (origin_str);
}

// 잘못된 환경변수 이름 입력했을 때 permission denied 해결하기
void	transform(char **tmp)
{
	int	idx;

	idx = 0;
	while (tmp[idx])
	{
		tmp[idx] = expand_env(tmp[idx]);
		idx++;
	}
}
