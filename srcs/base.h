/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 14:26:25 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/18 17:43:50 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASE_H
# define BASE_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/wait.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <sys/errno.h>
# include <sys/stat.h>
# include <termios.h>
# include "../libft/libft.h"

# define P_WRITE 1
# define P_READ 0

# define SUCCESS 0
# define FAILURE -1

# define TRUE 1
# define FALSE 0

# define EXPORT 1
# define ENV 2
# define UNSET 3
# define EXIT 4
# define CD 5
# define PWD 6
# define _ECHO 7

# define BRACE 1
# define AND 2
# define OR 3
# define PIPE 4
# define CMD 5
# define REDIRECTION 6
# define ERROR 7

# define LOAD -1

typedef struct s_pid
{
	pid_t			pid;
	struct s_pid	*next;
}	t_pid;

typedef struct s_wildcard
{
	char				*name;
	struct s_wildcard	*next;
}	t_wildcard;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_token
{
	char			*token;
	int				type;
	struct s_token	*left;
	struct s_token	*right;
}	t_token;

typedef struct s_data
{
	t_env			*env;
	t_token			*head;
	t_wildcard		*wildcard;
	struct termios	term;
	char			**cmd_arr;
	int				is_built_in;
	int				pipefd[2];
	int				prev_fd;
	int				in_fd;
	int				out_fd;
	int				is_pipe;
	int				pipe_count;
	struct s_pid	*pids;
}	t_data;

//main.c / set_beginning.c
int			exit_status(int status);
void		set_beginning(t_data *data, char *envp[]);
void		handler(int sig);
void		heredoc_handler(int sig);

//list_env.c / list_token.c / list_wildcard.c
t_env		*lst_new_env(char *name, char *value);
void		lst_env_add_back(t_env **head, t_env *new);
t_env		*lst_env_last(t_env *lst);
void		lst_env_free(t_env *lst);
void		lst_env_free_all(t_env *lst);
t_token		*lst_new_token(char *str, int *idx);
int			lst_token_add_back(t_token **head, t_token *new);
t_token		*lst_token_last(t_token *lst);
void		lst_token_free(t_token *lst);
void		lst_token_free_all(t_token *lst);
t_wildcard	*lst_new_wildcard(char *name);
int			wildcard_add_back(t_wildcard **head, t_wildcard *new);
t_wildcard	*lst_wildcard_last(t_wildcard *lst);
void		lst_wildcard_free(t_wildcard *lst);
void		lst_wildcard_free_all(t_wildcard *lst);

//is_symbol.c
int			is_redirection(char *str);
int			is_pipe(char *str);
int			is_symbol(char *str);
int			is_special_chr(char c);
int			is_brace(char *str);

//heredoc.c
int			open_heredoc(t_token *search);
void		find_heredoc(t_token *top);
void		unlink_heredoc(t_token *top);

//pipe_set.c / pipe_check_cmd.c
int			set_fd(t_token *unit, t_data *data);
char		**set_cmd(t_token *unit);
int			check_cmd(t_data *data, t_token *unit);
char		*make_real_path(char *path, char *command);
char		**get_paths(t_data *data);

//pipe_execute_tree.c / pipe_run.c
int			execute_tree(t_token *top, t_data *data);
void		run_unit(t_token *unit, t_data *data);

//pipe_util.c
void		reset_line_data(t_data *data);
char		*find_command_in_path(char *command, t_data *data);
int			chk_stat(char *path_command);
void		add_pid(t_data *data, pid_t	pid);
void		wait_all(t_data *data);

//builtin_func
int			run_builtin_func(t_data *data);
int			builtin_env(char **cmd_arr, t_data *data);
int			builtin_export(t_data *data, char **cmd_arr);
int			builtin_unset(t_data *data, char **cmd_arr);
int			builtin_exit(char **cmd_arr);
int			builtin_cd(t_data *data, char *dir);
int			builtin_pwd(void);
int			builtin_echo(t_data *data);

#endif