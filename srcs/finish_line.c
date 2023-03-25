/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finish_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojyang <seojyang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 19:15:43 by seojyang          #+#    #+#             */
/*   Updated: 2023/03/25 13:58:07 by seojyang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base.h"
#include "util.h"

static void	lst_tree_free_all(t_token *top);
static void	unlink_heredoc(t_token *top);

void	finish_line(char *str, t_data *data)
{
	free(str);
	free_arr((void **)data->cmd_arr);
	unlink_heredoc(data->head);
	lst_tree_free_all(data->head);
	data->head = 0;
	system("leaks --quiet minishell");
}

static void	lst_tree_free_all(t_token *top)
{
	if (top == 0)
		return ;
	lst_tree_free_all(top->left);
	lst_tree_free_all(top->right);
	lst_token_free(top);
}

static void	unlink_heredoc(t_token *top)
{
	char	*heredoc_tmp_name;

	if (top == 0)
		return ;
	unlink_heredoc(top->left);
	unlink_heredoc(top->right);
	if (ft_strncmp(top->token, "<<", 3) == 0)
	{
		heredoc_tmp_name = ft_itoa((unsigned long long) &top->token);
		unlink(heredoc_tmp_name);
		free(heredoc_tmp_name);
	}
}
