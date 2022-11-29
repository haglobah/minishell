#include "libft/libft.h"

int	main(int argc, char *argv[], char *envp[])
{
	int	i;
	int	n_flag;

	n_flag = 0;
	ft_printf("\n----------------\n");
	i = 1;
	if (!ft_strcmp(argv[1], "-n"))
	{
		i++;
		n_flag = 1;
	}
	while (argv[i])
	{
		ft_printf("%i: %s", i, argv[i]);
		if (!n_flag || (n_flag && i < argc - 1))
			ft_printf("\n");
		i++;
	}
	ft_printf("\n----------------\n");
}
