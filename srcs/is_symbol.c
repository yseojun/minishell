/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_symbol.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 19:32:06 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/25 21:17:35 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

int	is_redirection(char *str)
{
	return (!ft_strncmp(str, "<<", 3) || !ft_strncmp(str, "<", 2)
		|| !ft_strncmp(str, ">>", 3) || !ft_strncmp(str, ">", 2));
}

int	is_pipe(char *str)
{
	return (!ft_strncmp(str, "|", 2));
}

int	is_symbol(char *str)
{
	return (is_pipe(str) || is_redirection(str));
}

int	is_special_symbol(char c)
{
	return (c == '<' || c == '>' || c == '|' || c == '&');
}