/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 14:26:25 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/21 22:04:18 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASE_H
# define BASE_H

# include <stdio.h> // readline, 
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h> // wait
# include <readline/history.h>
# include <readline/readline.h> // readline
# include <sys/errno.h>
# include "../gnl/get_next_line_bonus.h"
# include "../libft/libft.h"

extern char	**environ;

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

typedef struct s_data
{
	char	*now_dir;
	int		last_child_status;
}	t_data;

//main.c
void		handler(int sig);
void		init_data(t_data *data);
void		finish_line(char *str, t_pipe *info);
void		wait_all(t_pipe *info);

//set_pipe.c
int			set_pipe(t_pipe *info, char **tmp);
int			chk_cmd(t_pipe *info);
char		**set_cmd(char **tmp);
int			count_cmd(char **tmp);
int			set_in_out(t_pipe *info, char **tmp, int idx);
int			is_redirection(char *str);

//pipe_info.c
void		init_pipe_info(t_pipe *info);
char		*make_real_path(char *path, char *command);
char		*find_command_in_path(char *command, char **path);
void		add_pid(t_pipe *info, pid_t	pid);

//pipe_infile.c
int			make_heredoc(char *limiter);
int			infile_chk(char *infile);

//run_pipe.c
int			run_pipe(t_pipe *info, t_data *data, int idx);

//user_func
char		*get_env(char *str);
int			_export(char *to_add);
int			_unset(char *to_del);
void		chk_user_func(t_pipe *info, t_data *data);

#endif