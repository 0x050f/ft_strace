#ifndef FT_STRACE_H
# define FT_STRACE_H

# define _GNU_SOURCE
# include <errno.h>
# include <sched.h>
# include <stdarg.h>
# include <stdbool.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/ptrace.h>
# include <sys/stat.h>
# include <sys/uio.h>
# include <sys/user.h>
# include <sys/wait.h>
# include <unistd.h>

# include "x86_64_syscall.h"
# include "sys_signame.h"

# define PRG_NAME "ft_strace"

/* exec.c */
char			*get_executable(char *name);

#endif
