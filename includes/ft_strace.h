#ifndef FT_STRACE_H
# define FT_STRACE_H

# include <errno.h>
# include <stdbool.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

# define PRG_NAME "ft_strace"

/* exec.c */
char			*get_executable(char *name);

#endif
