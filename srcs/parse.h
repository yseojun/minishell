/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rolee <rolee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 21:26:53 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/15 21:30:47 by rolee            ###   ########.fr       */
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

// transform - expand
char		*expand(t_data *data, char *str, int *flag);
char		*get_expanded(t_data *data, int dollar, char *str, int *idx);
char		*get_value(t_data *data, char *key);
void		handle_single_quote(char *str, int *idx);
char		*handle_double_quote(t_data *data, char *str, int *idx);
t_token		*pull_token(t_token **head, t_token *remove);

// transform - wildcard
void		cmp_wildcard(t_data *data, char **to_find, t_token *now);
int			merge_wildcard_lst(t_data *data, t_token **now);

// tree_make
t_token		*make_tree(t_token *tail);
t_token		*find_command(t_token *tail);

#endif