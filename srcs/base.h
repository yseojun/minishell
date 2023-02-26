/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 14:26:25 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/26 20:50:27 by seojyang         ###   ########.fr       */
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

# define P_WRITE 1
# define P_READ 0
# define CMD_NOT_FOUND "minishell: command not found: "
# define SUCCESS 0
# define FAILURE -1
# define TMP_FILE "tmp.txt"

extern char	**environ;

typedef struct s_pid
{
	pid_t			pid;
	struct s_pid	*next;
}	t_pid;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_pipe
{
	int				pipefd[2];
	int				prev_fd;
	char			**token_arr;
	int				token_arr_size;
	char			**unit;
	int				unit_size;
	char			**cmd_arr;
	int				in_fd;
	int				out_fd;
	char			**path;
	int				is_built_in;
	struct s_pid	*pids;
}	t_pipe;

typedef struct s_data
{
	int		last_child_status;
	t_env	*env;
}	t_data;

//main.c
void		handler(int sig);
void		finish_line(char *str, t_pipe *info);
void		wait_all(t_pipe *info);

//base_data.c
void		init_data(t_data *data);
t_env		*init_env(void);

//list_env.c
t_env		*lst_new_env(char *name, char *value);
void		lst_env_add_back(t_env **head, t_env *new);
t_env		*lst_env_last(t_env *lst);
void		lst_env_free(t_env *lst);


//set_pipe.c
int			chk_cmd(t_pipe *info);
char		**set_cmd(char **tmp);
int			count_cmd(char **tmp);
int			set_fd(t_pipe *info);

//pipe_info.c
void		init_pipe_info(t_pipe *info);
char		*make_real_path(char *path, char *command);
char		*find_command_in_path(char *command, char **path);
void		add_pid(t_pipe *info, pid_t	pid);

//pipe_infile.c
int			make_heredoc(char *limiter);
int			infile_chk(char *infile);

//run_pipe.c
int			run_unit(t_pipe *info, t_data *data);

//user_func
char		*get_env(t_data *data, char *str);
void		_export(t_data *data, char *token);
void		_unset(t_data *data, char *name);
void		run_user_func(t_pipe *info, t_data *data);

//is_symbol.c
int			is_redirection(char *str);
int			is_pipe(char *str);
int			is_symbol(char *str);

//token
int			tokenalize(char *str, t_pipe *info);

// transform
void		handle_single_quotes(char *str, int *idx);
char		*handle_double_quotes(t_data *data, char *str, int *idx, int *flag);
char		*expand_env(t_data *data, char *str, int *flag);
char		*get_expanded(t_data *data, int dollar_idx, \
							char *origin_str, int *idx);
void		transform(t_data *data, t_pipe *info);

#endif