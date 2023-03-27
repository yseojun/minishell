/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 21:26:53 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/27 20:27:44 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "base.h"

// parse.c
int			parse_line(char *str, t_data *data);

//chk_condition.c
int			chk_if_open_brace(t_token *now, int *brace_opened);
int			chk_if_close_brace(t_token *now, int *brace_opened);
int			chk_if_pipe_logic(t_token *now);
int			chk_if_redirection(t_token *now);
int			chk_if_cmd(t_token *now);

// tokenize.c
void		tokenize(char *str, t_data *data);
char		*put_token(char *str, int *idx);
int			set_type(char *token);

// transform
void		transform(t_data *data, t_token *top);
char		*remove_quote(char *str);
int			get_quote_size(char *str, int idx);

// transform - expand
char		*expand(t_data *data, char *str, int *flag);
char		*get_expanded(t_data *data, int dollar, char *str, int *idx);
char		*get_value(t_data *data, char *key);
void		handle_single_quote(char *str, int *idx);
char		*handle_double_quote(t_data *data, char *str, int *idx);

// transform - wildcard
void		wildcard(t_data *data, t_token **search);
void		cmp_wildcard(t_data *data, t_wildcard *to_find, t_token *now);
void		wildcard_merge(t_data *data, t_token *now);
t_wildcard	*make_to_find_head(char *s);

// tree_make
t_token		*make_tree(t_token *tail);
t_token		*find_command(t_token *tail);
t_token		*find_braces(t_token *tail);

#endif