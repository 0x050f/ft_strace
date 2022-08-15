#include "ft_strace.h"

static char		*prg_name;

char			*get_full_path(char *exec, char *path)
{
	char		*ptr;
	char		*exec_full_path;
	int			path_size;
	int			len;
	struct stat	statbuf;

	if (!exec || !path)
		return (NULL);
	ptr = strtok(path, ":");
	while (ptr)
	{
		len = strlen(ptr);
		path_size = len + strlen(exec) + 1;
		if (ptr[len - 1] != '/')
			path_size++;
		exec_full_path = malloc(sizeof(char) * path_size);
		if (!exec_full_path)
			return (NULL);
		memset(exec_full_path, 0, sizeof(char) * path_size);
		strcpy(exec_full_path, ptr);
		if (ptr[len - 1] != '/')
			exec_full_path[len] = '/';
		strcat(exec_full_path, exec);
		if (!stat(exec_full_path, &statbuf))
			return (exec_full_path);
		free(exec_full_path);
		ptr = strtok(NULL, ":");
	}
	return (NULL);
}

char			*get_executable(char *name)
{
	char	*path;
	char	*exec;

	path = getenv("PATH");
	if (!path)
		return (NULL);
	if (!strchr(name, '/'))
		exec = get_full_path(name, path);
	else
		exec = strdup(name);
	if (!exec)
		return (NULL);
	return (exec);
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

int				main(int argc, char *argv[])
{
	char	*exec;

	if (!check_args(argc, argv))
		return (EXIT_FAILURE);
	exec = get_executable(argv[1]);
	if (!exec)
	{
		fprintf(stderr, "%s: %s: %s\n", prg_name, argv[1], strerror(errno));
		free(prg_name);
		return (EXIT_FAILURE);
	}
	free(exec);
	free(prg_name);
	return (EXIT_SUCCESS);
}
