#include <stdio.h>
#include <unistd.h>

int	main(void)
{
	char	*argv[] = { "jim", "jams", NULL };
	char	*envp[] = { "some", "environment", NULL };

	printf("Main program started\n");
	if (execve("./bla", argv, envp) == -1)
		perror("Could not execve");
	return 1;
}
