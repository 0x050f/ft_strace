#include "ft_strace.h"

char			*get_full_path(char *exec, char *path)
{
	char		*ptr;
	char		*exec_full_path;
	int			path_size;
	int			len;
	struct stat	sb;

	if (!exec || !path)
	{
		errno = ENOENT;
		return (NULL);
	}
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
		if (!stat(exec_full_path, &sb) &&
((sb.st_mode & S_IFMT) == S_IFLNK || (sb.st_mode & S_IFMT) == S_IFREG))
			return (exec_full_path);
		free(exec_full_path);
		ptr = strtok(NULL, ":");
	}
	return (NULL);
}

char			*get_executable(char *name)
{
	struct stat	sb;
	char		*path;
	char		*exec;

	if (!strchr(name, '/'))
	{
		path = getenv("PATH");
		exec = get_full_path(name, path);
	}
	else
		exec = strdup(name);
	if (stat(exec, &sb) ||
((sb.st_mode & S_IFMT) != S_IFLNK && (sb.st_mode & S_IFMT) != S_IFREG))
	{
		errno = ENOENT;
		return (NULL);
	}
	return (exec);
}
