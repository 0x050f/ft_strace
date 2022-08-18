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
# define LONG				2
# define ULONG				3
# define PTR				4
# define STR				5
# define FLAG_OPEN			6
# define FLAG_OPENAT		7
# define FLAG_PROT			8
# define FLAG_MMAP			9
# define STRUCT_STAT		10
# define STRUCT_POLL		11
# define STRUCT_SIGACT		12
# define STRUCT_SIGSET		13
# define STRUCT_SIGINF		14
# define STRUCT_IOVEC		15
# define STRUCT_FDSET		16
# define STRUCT_TIMEVAL		17
# define STRUCT_TIMEZONE	18
# define STRUCT_TIMESPEC	19
# define STRUCT_SHMID		20
# define STRUCT_SOCKADDR	21
# define STRUCT_MSGHDR		22
# define STRUCT_RUSAGE		23
# define STRUCT_UTSNAME		24
# define STRUCT_SEMBUF		25
# define STRUCT_MSGID		26
# define STRUCT_LINUX_DIR	27
# define STRUCT_RLIMIT		28
# define STRUCT_SYSINFO		29
# define STRUCT_TMS			30
# define PIPE				31
# define SV					32
# define KEY				33
# define MODE				34
# define CLOCK				35
# define PTRACE				36
# define ID_T				37
# define DEV				38
# define TIME				39
# define ARGV				40
# define ENVP				41

#endif
