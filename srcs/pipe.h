/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:14:09 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/18 19:43:50 by seojyang         ###   ########.fr       */
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
	int				infile_fd;
	int				outfile_fd;
	char			*limiter;
	char			**path;
	struct s_pid	*pids;
}	t_pipe;

void	run_pipe(t_pipe *info);
void	run_command(t_pipe *info);
void	child(int *pipefd, int prev_fd, t_pipe *info);
void	wait_all(t_pipe *info);

void	init_info(t_pipe *info, char **envp);
char	**find_path_in_env(char **envp);
char	*make_real_path(char *path, char *command);
char	*find_command_in_path(char *command, char **path);
void	add_pid(t_pipe *info, pid_t	pid);

#endif