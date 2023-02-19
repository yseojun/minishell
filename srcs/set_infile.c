#include "libft.h"

int	set_infile(char **tmp, int prev_fd)
{
	int	fd;
	int	idx;
	int	is_infile;

	is_infile = 0;
	idx = 0;
	while (tmp[idx])
	{
		if (!ft_strncmp(tmp[idx], "<<", 3))
		{
			is_infile = 1;
			fd = make_heredoc();
		}
		else if (!fd_strncmp(tmp[idx], "<", 2))
		{
			is_infile = 1;
			fd = infile_chk(tmp[idx + 1]);
		}
		idx++;
		if (fd < 0)
			return (fd);
	}
	if (is_infile == 0)
		return (prev_fd);
	else
		return (fd);
}
