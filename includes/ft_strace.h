#ifndef FT_STRACE_H
# define FT_STRACE_H

# define _GNU_SOURCE
# include <elf.h>
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

# include "syscall.h"
# include "sys_signame.h"

# define PRG_NAME "ft_strace"

struct i386_user_regs_struct {
	int		ebx;
	int		ecx;
	int		edx;
	int		esi;
	int		edi;
	int		ebp;
	int		eax;
	int		xds;
	int		xes;
	int		xfs;
	int		xgs;
	int		orig_eax;
	int		eip;
	int		xcs;
	int		eflags;
	int		esp;
	int		xss;
};

typedef struct	syscall_handle_s {
	bool		start;
	bool		print;
	bool		result;
	int			mode;
}				syscall_handle_t;

/* exec.c */
char			*get_executable(char *name);

/* syscall.c */
int				get_syscalls(pid_t pid);

#endif
