#include "../libft/libft.h"
#include <fcntl.h>
#include "pipe.h"

int	chk_cmd(t_pipe *info);
char	**set_cmd(char **tmp);
int	count_cmd(char **tmp);
int	set_in_out(t_pipe *info, char **tmp, int idx);
int	is_redirection(char *str);

int	set_pipe(t_pipe *info, char **tmp)
{
	int	fd;
	int	idx;
	int	chk;

	idx = 0;
	fd = 0;
	chk = 0;
	info->cmd = set_cmd(tmp);
	while (tmp[idx])
	{
		if (is_redirection(tmp[idx]))
		{
			if (set_in_out(info, tmp, idx) < 0)
				return (-1);
			idx++;
		}
		else if (chk == 0)
		{
			if (chk_cmd(info) < 0)
				return (-1);
			chk = 1;
		}
		idx++;
	}
	return (0);
}

int	chk_cmd(t_pipe *info)
{
	int		idx;
	char	*tmp_path;

	idx = 0;
	while (info->path[idx])
	{
		tmp_path = make_real_path(info->path[idx], info->cmd[0]);
		if (access(tmp_path, F_OK) == 0)
		{
			free(tmp_path);
			return (0);
		}
		idx++;
		free(tmp_path);
	}
	if (access(info->cmd[0], F_OK) == 0)
		return (0);
	ft_putstr_fd("minishell: command not found: ", 2); // sig?
	ft_putendl_fd(info->cmd[0], 2);
	return (-1);
}

char	**set_cmd(char **tmp)
{
	char	**cmd;
	int		idx;
	int		count;

	count = count_cmd(tmp);
	if (count == 0)
		return (0);
	cmd = (char **)malloc(sizeof(char *) * (count + 1));
	if (!cmd)
		exit(1);
	idx = 0;
	while (tmp[idx])
	{
		if (is_redirection(tmp[idx]))
			idx++;
		else
			cmd[idx] = ft_strdup(tmp[idx]);
		idx++;
	}
	return (cmd);
}

int	count_cmd(char **tmp)
{
	int	idx;
	int	count;

	idx = 0;
	count = 0;
	while (tmp[idx])
	{
		if (is_redirection(tmp[idx]))
			idx++;
		else
			count++;
		idx++;
	}
	return (count);
}

int	set_in_out(t_pipe *info, char **tmp, int idx)
{
	if (!ft_strncmp(tmp[idx], "<<", 3))
		info->infile_fd = make_heredoc(tmp[idx + 1]);
	else if (!ft_strncmp(tmp[idx], "<", 2))
		info->infile_fd = infile_chk(tmp[idx + 1]);
	else if (!ft_strncmp(tmp[idx], ">>", 3))
		info->outfile_fd = \
		open(tmp[idx + 1], O_WRONLY | O_APPEND | O_CREAT, 0644);
	else if (!ft_strncmp(tmp[idx], ">", 2))
		info->outfile_fd = \
		open(tmp[idx + 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (info->infile_fd < 0 || info->outfile_fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(tmp[idx + 1]); // signal?
		return (-1);
	}
	return (0);
}

int	is_redirection(char *str)
{
	return (!ft_strncmp(str, "<<", 3) || !ft_strncmp(str, "<", 2)
		|| !ft_strncmp(str, ">>", 3) || !ft_strncmp(str, ">", 2));
}
