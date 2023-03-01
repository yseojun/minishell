/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 20:21:24 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/01 19:49:48 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "../gnl/get_next_line_bonus.h"

int	make_heredoc(char *limiter)
{
	char	*str;
	char	*cmp_limiter;
	int		tmp_fd;

	str = 0;
	cmp_limiter = ft_strjoin(limiter, "\n");
	tmp_fd = open(TMP_FILE, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	while (1)
	{
		ft_putstr_fd("pipe heredoc> ", STDOUT_FILENO);
		str = get_next_line(STDIN_FILENO);
		if (!str || ft_strncmp(str, cmp_limiter, ft_strlen(str) + 1) == 0)
			break ;
		write(tmp_fd, str, ft_strlen(str));
		free(str);
		str = 0;
	}
	free(cmp_limiter);
	free(str);
	close(tmp_fd);
	tmp_fd = open(TMP_FILE, O_RDONLY, 0644);
	return (tmp_fd);
}

int	infile_chk(char *infile)
{
	int	fd;

	fd = open(infile, O_RDONLY);
	return (fd);
}
