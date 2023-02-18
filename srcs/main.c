/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 14:25:04 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/18 15:16:09 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"

int	main(int ac, char **av, char **envp)
{
	char	*str;

	ac = 1;
	envp = 0;
	av = 0;
	while (1)
	{
		ft_putstr_fd("minishell> ", 1);
		str = get_next_line(0);
		if (!str)
			break ;
		write(1, str, ft_strlen(str));
		free(str);
		str = 0;
	}
}
