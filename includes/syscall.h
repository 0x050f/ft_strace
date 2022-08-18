#ifndef SYSCALL_H
# define SYSCALL_H

# include "x86_64_syscall.h"
# include "i386_syscall.h"


# define MAX_ARGS 6

typedef struct	syscall_s {
	char		*name;
	int			argc;
	int			type_args[MAX_ARGS];
	int			type_ret;
}				syscall_t;

# define MODE_64BITS		0
# define MODE_32BITS		1

/* TODO: More flags */
# define INT				1
# define ULONG				2
# define PTR				3
# define STR				4
# define FLAG_OPEN			5
# define FLAG_OPENAT		6
# define FLAG_PROT			7
# define FLAG_MMAP			8
# define STRUCT_STAT		9
# define STRUCT_POLL		10
# define STRUCT_SIGACT		11
# define STRUCT_SIGSET		12
# define STRUCT_SIGINF		13
# define STRUCT_IOVEC		14
# define STRUCT_FDSET		15
# define STRUCT_TIMEVAL		16
# define STRUCT_TIMEZONE	17
# define STRUCT_TIMESPEC	18
# define STRUCT_SHMID		19
# define STRUCT_SOCKADDR	20
# define STRUCT_MSGHDR		21
# define STRUCT_RUSAGE		22
# define STRUCT_UTSNAME		23
# define STRUCT_SEMBUF		24
# define STRUCT_MSGID		25
# define STRUCT_LINUX_DIR	26
# define STRUCT_RLIMIT		27
# define STRUCT_SYSINFO		28
# define STRUCT_TMS			29
# define PIPE				30
# define SV					31
# define KEY				32
# define MODE				33
# define CLOCK				34
# define PTRACE				35
# define ID_T				36
# define ARGV				37
# define ENVP				38



#endif
