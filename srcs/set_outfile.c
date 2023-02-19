#include "libft.h"
#include "base.h"

int	set_outfile(char **tmp)
{
	int	fd;
	int	idx;

	fd = STDOUT_FILENO;
	idx = 0;
	while (tmp[idx])
	{
		if (!ft_strncmp(tmp[idx], ">>", 3))
			fd = open(tmp[idx + 1], O_WRONLY | O_APPEND | O_CREAT, 0644);
		else if (!fd_strncmp(tmp[idx], ">", 2))
			fd = open(tmp[idx + 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
		idx++;
	}
	return (fd);
}