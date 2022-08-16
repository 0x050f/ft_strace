#include "ft_strace.h"

static char		*prg_name;

int				strace(char *exec, char *argv[], char *envp[])
{
	int		status;
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		fprintf(stderr, "%s: fork: %s\n", prg_name, strerror(errno));
		return (EXIT_FAILURE);
	}
	else if (!pid)
	{
		if (execve(exec, argv, envp) < 0)
		{
			fprintf(stderr, "%s: exec: %s\n", prg_name, strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(&status);
		fprintf(stderr, "+++ exited with %d +++\n", WEXITSTATUS(status));
	}
	return (WEXITSTATUS(status));
}

bool			check_args(int argc, char *argv[])
{
	prg_name = strdup((*argv[0]) ? argv[0] : PRG_NAME);
	if (!prg_name)
	{
		fprintf(stderr, "%s: %s\n", PRG_NAME, strerror(errno));
		return (false);
	}
	if (argc < 2)
	{
		fprintf(stderr, "usage: %s prg [args]\n", prg_name);
		free(prg_name);
		return (false);
	}
	return (true);
}

int				main(int argc, char *argv[], char *envp[])
{
	int		ret;
	char	*exec;

	if (!check_args(argc, argv))
		return (EXIT_FAILURE);
	exec = get_executable(argv[1]);
	if (!exec)
	{
		fprintf(stderr, "%s: '%s': %s\n", prg_name, argv[1], strerror(errno));
		free(prg_name);
		return (EXIT_FAILURE);
	}
	ret = strace(exec, &argv[1], envp);
	free(exec);
	free(prg_name);
	return (ret);
}
