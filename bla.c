#include <stdio.h>

void	prints(char **slist)
{
	int	i;

	i = -1;
	while (slist[++i])
	{
		printf("%i: %s\n", i, slist[i]);
	}
}

void	printa(int argc, char **args)
{
	int	i;

	i = -1;
	while (++i < argc)
	{
		printf("%i: %s\n", i, args[i]);
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	printa(argc, argv);
//	printf("%s\n", envp[8]);
}
