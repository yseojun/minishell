/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 20:21:24 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/21 21:57:43 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "../gnl/get_next_line_bonus.h"

int	make_heredoc(char *limiter)
{
	char	*str;
	int		tmp_fd;

	str = 0;
	tmp_fd = open("tmp.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	while (1)
	{
		ft_putstr_fd("pipe heredoc> ", 1);
		str = get_next_line(0);
		if (!str || ft_strncmp(str, limiter, ft_strlen(str)) == 0)
			break ;
		write(tmp_fd, str, ft_strlen(str));
		free(str);
		str = 0;
	}
	free(str);
	close(tmp_fd);
	tmp_fd = open("tmp.txt", O_RDONLY, 0644);
	return (tmp_fd);
}

int	infile_chk(char *infile)
{
	int	fd;

	fd = open(infile, O_RDONLY);
	return (fd);
}
