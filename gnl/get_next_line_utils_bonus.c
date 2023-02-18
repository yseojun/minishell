/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 20:16:18 by seojyang          #+#    #+#             */
/*   Updated: 2023/01/31 16:58:05 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

t_line	find_save(t_gnl_list *list, int fd)
{
	t_line	new_fd;

	while (list)
	{
		if (fd == list->fd)
			return (list->line);
		list = list->next;
	}
	new_fd.buffer = 0;
	new_fd.read_idx = 0;
	new_fd.read_max_len = 0;
	return (new_fd);
}

void	remove_lst(t_gnl_list **list, int fd)
{
	t_gnl_list	*save;
	t_gnl_list	*search;

	save = *list;
	search = *list;
	while (search)
	{
		if (search->fd == fd)
		{
			save->next = search->next;
			if (save == search)
			{
				if (!search->next)
					*list = 0;
				else
					*list = search->next;
			}
			free(search);
			search = 0;
			return ;
		}
		save = search;
		search = search->next;
	}
}

char	*save_line(char *output, t_gnl_list **list, t_line line, int fd)
{
	t_gnl_list	*new;
	t_gnl_list	*search;

	search = *list;
	while (search)
	{
		if (search->fd == fd)
		{
			search->line = line;
			return (output);
		}
		if (search->next == 0)
			break ;
		search = search->next;
	}
	new = (t_gnl_list *)malloc(sizeof(t_gnl_list));
	if (!new)
		return (read_finish(&line, output, list, fd));
	init_list(new, line, fd);
	if (!search)
		*list = new;
	else
		search->next = new;
	return (output);
}

void	init_list(t_gnl_list *new, t_line line, int fd)
{
	new->fd = fd;
	new->line = line;
	new->next = 0;
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	idx;

	if (!dst && !src)
		return (0);
	idx = 0;
	while (idx < n)
	{
		*((unsigned char *) dst + idx) = *((unsigned char *) src + idx);
		idx++;
	}
	return (dst);
}
