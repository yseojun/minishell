/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 14:26:25 by seojyang          #+#    #+#             */
/*   Updated: 2023/02/18 16:57:27 by seojyang         ###   ########.fr       */
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

typedef struct s_data
{
	char	**history;
	char	**path;
	char	*now;
	int		status;
	char	**tmp;
	int		tmp_size;
	char	**command;
	int		instruction;
}	t_data;

typedef struct s_pid
{
	int				pid;
	struct s_pid	next;
}	t_pid

#endif