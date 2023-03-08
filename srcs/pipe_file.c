/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojun <seojun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 20:21:24 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/08 15:41:22 by seojun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "../gnl/get_next_line_bonus.h"

void	write_heredoc(int heredoc_fd, char *limiter)
{
	char	*str;
	char	*cmp_limiter;

	str = 0;
	cmp_limiter = ft_strjoin(limiter, "\n");
	while (1)
	{
		ft_putstr_fd("pipe heredoc> ", STDOUT_FILENO);
		str = get_next_line(STDIN_FILENO);
		if (!str || ft_strncmp(str, cmp_limiter, ft_strlen(str) + 1) == 0)
			break ;
		write(heredoc_fd, str, ft_strlen(str));
		free(str);
		str = 0;
	}
	free(cmp_limiter);
	free(str);
}

int	make_heredoc(char *limiter, t_pipe *info)
{
	int		heredoc_fd;
	char	*tmp_name;

	tmp_name = ft_itoa((long long) &tmp_name);
	heredoc_fd = open(tmp_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (heredoc_fd == -1)
	{
		free(tmp_name);
		tmp_name = 0;
		return (FAILURE);
	}
	info->heredoc_tmp = tmp_name;
	write_heredoc(heredoc_fd, limiter);
	close(heredoc_fd);
	heredoc_fd = open(tmp_name, O_RDONLY, 0644);
	return (heredoc_fd);
}

int	infile_chk(char *infile)
{
	int	fd;

	fd = open(infile, O_RDONLY);
	return (fd);
}
