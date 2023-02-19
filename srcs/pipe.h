/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:14:09 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/19 16:52:18 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPE_H
# define PIPE_H

# include "../libft/libft.h"
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <errno.h>
# include <stdio.h>

typedef struct s_pid
{
	pid_t			pid;
	struct s_pid	*next;
}	t_pid;

typedef struct s_pipe
{
	int				pipefd[2];
	int				prev_fd;
	int				tmp_size;
	char			**tmp;
	char			**cmd;
	int				infile_fd;
	int				outfile_fd;
	int				in_fd;
	int				out_fd;
	int				is_valid_cmd;
	char			**path;
	struct s_pid	*pids;
}	t_pipe;

int		set_pipe(t_pipe *info, char **tmp);
int		run_pipe(t_pipe *info, int idx);
void	run_command(t_pipe *info);
void	child(t_pipe *info);
void	wait_all(t_pipe *info);

void	init_info(t_pipe *info);
char	*make_real_path(char *path, char *command);
char	*find_command_in_path(char *command, char **path);
void	add_pid(t_pipe *info, pid_t	pid);
int		make_heredoc(char *limiter);
int		infile_chk(char *infile);

#endif