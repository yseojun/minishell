/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 21:26:53 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/18 13:15:00 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "base.h"

// parse.c
int			parse_line(char *str, t_data *data);

// tokenize.c
int			tokenize(char *str, t_data *data);
char		*put_token(char *str, int *idx);
int			set_type(char *token);

// transform
int			transform(t_data *data);
char		*remove_quote(char *str);
int			get_qoute_size(char *str, int idx);

// transform - expand
char		*expand(t_data *data, char *str, int *flag);
char		*get_expanded(t_data *data, int dollar, char *str, int *idx);
char		*get_value(t_data *data, char *key);
void		handle_single_quote(char *str, int *idx);
char		*handle_double_quote(t_data *data, char *str, int *idx);

// transform - wildcard
int			wildcard(t_data *data, t_token **search);
void		cmp_wildcard(t_data *data, t_wildcard *to_find, t_token *now);
int			merge_wildcard_lst(t_data *data, t_token **now);
t_wildcard	*make_to_find_head(char *s);

// tree_make
t_token		*make_tree(t_token *tail);
t_token		*find_command(t_token *tail);

#endif