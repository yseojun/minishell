/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 20:21:24 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/14 12:42:05 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "../gnl/get_next_line_bonus.h"

static void	write_heredoc(int heredoc_fd, char *limiter);

int	open_heredoc(t_token *search)
{
	char	*heredoc_tmp_name;
	int		fd;

	heredoc_tmp_name = ft_itoa((unsigned long long) &search->token);
	fd = open(heredoc_tmp_name, O_RDONLY);
	free(heredoc_tmp_name);
	return (fd);
}

void	find_heredoc(t_token *top)
{
	int		heredoc_fd;
	char	*heredoc_tmp_name;

	if (top == 0)
		return ;
	find_heredoc(top->left);
	find_heredoc(top->right);
	if (ft_strncmp(top->token, "<<", 3) == 0)
	{
		heredoc_tmp_name = ft_itoa((unsigned long long) &top->token);
		heredoc_fd = open(heredoc_tmp_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (heredoc_fd == -1)
		{
			free(heredoc_tmp_name);
			heredoc_tmp_name = 0;
			return ;
		}
		free(heredoc_tmp_name);
		write_heredoc(heredoc_fd, top->right->token);
		close(heredoc_fd);
	}
}

void	unlink_heredoc(t_token *top)
{
	char	*heredoc_tmp_name;

	if (top == 0)
		return ;
	unlink_heredoc(top->left);
	unlink_heredoc(top->right);
	if (ft_strncmp(top->token, "<<", 3) == 0)
	{
		heredoc_tmp_name = ft_itoa((unsigned long long) &top->token);
		unlink(heredoc_tmp_name);
		free(heredoc_tmp_name);
	}
}

static void	write_heredoc(int heredoc_fd, char *limiter)
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
