/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 20:15:33 by seojyang          #+#    #+#             */
/*   Updated: 2023/01/31 16:58:19 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*get_next_line(int fd)
{
	static t_gnl_list	*list = 0;
	t_line				line;
	char				*output;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (0);
	line = find_save(list, fd);
	line.output_len = 0;
	output = 0;
	if (!line.buffer)
		line.buffer = malloc(BUFFER_SIZE);
	while (1)
	{
		if (line.read_idx == line.read_max_len)
		{
			line.read_idx = 0;
			line.read_max_len = read(fd, line.buffer, BUFFER_SIZE);
		}
		if (line.read_max_len <= 0)
			return (read_finish(&line, output, &list, fd));
		output = join(output, &line);
		if (line.output_len && *(output + line.output_len - 1) == '\n')
			return (save_line(output, &list, line, fd));
	}
}

char	*join(char *output, t_line *line)
{
	char	*new;
	ssize_t	join_len;

	join_len = 0;
	while (line->read_idx + join_len < line->read_max_len)
	{
		if (*(line->buffer + line->read_idx + join_len) == '\n')
		{
			join_len++;
			break ;
		}
		join_len++;
	}
	new = malloc(line->output_len + join_len + 1);
	if (!new)
		return (0);
	ft_memcpy(new, output, line->output_len);
	ft_memcpy(new + line->output_len, line->buffer + line->read_idx, join_len);
	new[line->output_len + join_len] = 0;
	line->output_len += join_len;
	line->read_idx += join_len;
	free(output);
	return (new);
}

char	*read_finish(t_line *line, char *output, t_gnl_list **list, int fd)
{
	if (line->read_max_len < 0)
	{
		if (output)
			free(output);
		output = 0;
		line->read_idx = 0;
		line->read_max_len = 0;
	}
	free(line->buffer);
	line->buffer = 0;
	remove_lst(list, fd);
	return (output);
}
