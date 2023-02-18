/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 20:21:24 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/18 19:18:19 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe.h"
#include "get_next_line_bonus.h"

int	heredoc_chk(char *av1)
{
	return (ft_strncmp("here_doc", av1, ft_strlen(av1)) == 0);
}

int	make_heredoc(t_pipe *info)
{
	char	*str;
	int		tmp_fd;

	str = 0;
	tmp_fd = open("tmp.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	while (1)
	{
		ft_putstr_fd("pipe heredoc> ", 1);
		str = get_next_line(0);
		if (!str || ft_strncmp(str, info->limiter, ft_strlen(str)) == 0)
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
	if (fd < 0)
	{
		ft_putstr_fd("pipex: ", 2);
		perror_exit(infile);
	}
	return (fd);
}

int	outfile_chk(char *outfile, t_pipe *info)
{
	int	fd;
	int	option;

	if (info->heredoc == 1)
		option = O_WRONLY | O_APPEND | O_CREAT;
	else
		option = O_WRONLY | O_TRUNC | O_CREAT;
	fd = open(outfile, option, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("pipex: ", 2);
		perror_exit(outfile);
	}
	return (fd);
}

void	perror_exit(char *name)
{
	perror(name);
	exit(1);
}
