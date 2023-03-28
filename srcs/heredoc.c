/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 20:21:24 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/28 10:18:09 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "parse.h"
#include "util.h"

static int	make_heredoc(t_token *top);
static int	write_heredoc(int heredoc_fd, char *limiter);
static void	heredoc_child(int heredoc_fd, char *limiter);

int	find_heredoc(t_token *top)
{
	if (top == 0)
		return (EXIT_SUCCESS);
	if (ft_strncmp(top->token, "<<", 3) == 0)
	{
		top->right->token = remove_quote(top->right->token);
		if (make_heredoc(top) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	if (find_heredoc(top->left) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (find_heredoc(top->right) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	make_heredoc(t_token *top)
{
	int		heredoc_fd;
	char	*heredoc_tmp_name;
	char	*tmp;

	heredoc_tmp_name = ft_itoa((unsigned long long) &top->token);
	tmp = heredoc_tmp_name;
	heredoc_tmp_name = ft_strjoin("/tmp/", heredoc_tmp_name);
	free(tmp);
	heredoc_fd = open(heredoc_tmp_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (heredoc_fd == -1)
	{
		free(heredoc_tmp_name);
		heredoc_tmp_name = 0;
		return (EXIT_FAILURE);
	}
	free(heredoc_tmp_name);
	if (write_heredoc(heredoc_fd, top->right->token) != EXIT_SUCCESS)
	{
		close(heredoc_fd);
		return (EXIT_FAILURE);
	}
	close(heredoc_fd);
	return (EXIT_SUCCESS);
}

static int	write_heredoc(int heredoc_fd, char *limiter)
{
	pid_t		pid;
	int			status;

	pid = _fork();
	if (pid == 0)
		heredoc_child(heredoc_fd, limiter);
	signal(SIGINT, heredoc_handler);
	wait3(&status, 0, 0);
	exit_status(EXIT_SUCCESS);
	if (status != EXIT_SUCCESS)
		exit_status(EXIT_FAILURE * 256);
	signal(SIGINT, handler);
	return (exit_status(LOAD));
}

static void	heredoc_child(int heredoc_fd, char *limiter)
{
	char	*str;
	char	*tmp;

	signal(SIGINT, SIG_DFL);
	str = 0;
	while (1)
	{
		str = readline("heredoc> ");
		if (!str || ft_strncmp(str, limiter, ft_strlen(str) + 1) == 0)
			break ;
		tmp = str;
		str = ft_strjoin(str, "\n");
		free(tmp);
		write(heredoc_fd, str, ft_strlen(str));
		free(str);
		str = 0;
	}
	free(str);
	exit(EXIT_SUCCESS);
}

int	open_heredoc(t_token *search)
{
	char	*heredoc_tmp_name;
	char	*tmp;
	int		fd;

	heredoc_tmp_name = ft_itoa((unsigned long long) &search->token);
	tmp = heredoc_tmp_name;
	heredoc_tmp_name = ft_strjoin("/tmp/", heredoc_tmp_name);
	free(tmp);
	fd = open(heredoc_tmp_name, O_RDONLY);
	free(heredoc_tmp_name);
	return (fd);
}
