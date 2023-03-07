/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 14:26:25 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/07 20:06:43 by rolee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASE_H
# define BASE_H

# include <stdio.h> 
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/wait.h> // wait
# include <readline/history.h>
# include <readline/readline.h> // readline
# include <sys/errno.h>
# include <termios.h> // tcgetattr
# include "../gnl/get_next_line_bonus.h"
# include "../libft/libft.h"

# define P_WRITE 1
# define P_READ 0
# define CMD_NOT_FOUND "minishell: command not found: "
# define SUCCESS 0
# define FAILURE -1
# define TRUE 1
# define FALSE 0
# define TMP_FILE "tmp.txt"
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

extern char	**environ;

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

typedef struct s_pipe
{
	int				pipefd[2];
	int				prev_fd;
	t_token			*head;
	char			**token_arr;
	int				token_arr_size;
	t_token			**unit;
	int				unit_size;
	t_wildcard		*wildcard;
	char			**cmd_arr;
	int				in_fd;
	int				out_fd;
	int				is_built_in;
	int				is_pipe;
	char			*heredoc_tmp;
	struct s_pid	*pids;
}	t_pipe;

typedef struct s_data
{
	int				exit_status;
	t_env			*env;
	struct termios	termios;
}	t_data;

//base_data.c
void		init_data(t_data *data);
t_env		*init_env(void);

//list_env.c
t_env		*lst_new_env(char *name, char *value);
void		lst_env_add_back(t_env **head, t_env *new);
t_env		*lst_env_last(t_env *lst);
void		lst_env_free(t_env *lst);

//set_pipe.c
int			chk_cmd(t_pipe *info, t_data *data);
char		**set_cmd(t_token *unit);
int			count_cmd(t_token *unit);
int			set_fd(t_token *unit, t_pipe *info);

//pipe_info.c
void		init_pipe_info(t_pipe *info);
char		*make_real_path(char *path, char *command);
char		**get_paths(t_data *data);
char		*find_command_in_path(char *command, t_data *data);
void		add_pid(t_pipe *info, pid_t	pid);
void		wait_all(t_pipe *info, t_data *data);

//pipe_infile.c
int			make_heredoc(char *limiter, t_pipe *info);
int			infile_chk(char *infile);

//run_pipe.c
int 	excute_tree(t_token *top, t_pipe *info, t_data *data);
int		run_unit(t_token *unit, t_pipe *info, t_data *data);


//builtin_func.c
void		run_builtin_func(t_pipe *info, t_data *data);
void		builtin_exit(int status, t_data *data);

//is_symbol.c
int			is_redirection(char *str);
int			is_pipe(char *str);
int			is_symbol(char *str);
int			is_special_chr(char c);
int			is_brace(char *str);
int			is_brace_chr(char c);

//token
int			tokenize(char *str, t_pipe *info);
t_token		*lst_new_token(char *str);
int			token_add_back(t_token **head, t_token *new);
t_token		*lst_token_last(t_token *lst);
void		lst_token_free(t_token *lst);
t_token		*make_token(char *str, int *idx);
void		token_prt(t_token *head);
void		prt_tree(t_token *head);
char		*put_token(char *str, int *idx);
int			set_type(char *token);


#endif