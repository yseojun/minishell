/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 21:26:53 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/14 15:22:26 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "base.h"

// parse.c
int		    parse_line(char *str, t_data *data);

// tokenize.c
int		    tokenize(char *str, t_data *data);

// transform.c
int		    transform(t_data *data);

// transform_expand.c
char	    *expand(t_data *data, char *str, int *flag);
char	    *get_expanded(t_data *data, int dollar_idx, char *origin_str, int *idx);
char		*get_value(t_data *data, char *key);

// transform_util.c
void	    handle_single_quote(char *str, int *idx);
char	    *handle_double_quote(t_data *data, char *str, int *idx);
t_token	    *pull_token(t_token **head, t_token *remove);

// transform_rm_quote.c
char	    *remove_quote(char *str);

t_token     *make_tree(t_token *tail);

//wildcard.c
void		make_wildcard_lst(t_data *data, t_token *now);
int 		merge_wildcard_lst(t_data *data, t_token **now);

#endif