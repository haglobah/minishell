#include "libft/libft.h"
#include <fcntl.h>

void	printstdin(int fd)
{
	char	*i;

	i = get_next_line(fd);
	while (i)
	{
		ft_printf("%s", i);
		free(i);
		i = get_next_line(fd);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	int	fd = 0;

	if (argc == 2)
	{
		fd = open(argv[1], O_RDONLY);
	}
	ft_printf("\n----------------\n");
	printstdin(fd);
	ft_printf("\n----------------\n");
	return(12);
}
