#ifndef X86_64_SYSCALL_H
# define X86_64_SYSCALL_H

#define MAX_ARGS 6

typedef struct	x86_64_syscall_s {
	char		*name;
	int			argc;
	int			type_args[MAX_ARGS];
	int			type_ret;
}				x86_64_syscall_t;

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

# define X86_64_SYSCALL { \
[  0] = {"read", 3, {INT, PTR, INT, 0, 0, 0}, INT}, \
[  1] = {"write", 3, {INT, STR, INT, 0, 0, 0}, INT}, \
[  2] = {"open", 3, {STR, FLAG_OPEN, 0, 0, 0, 0}, INT}, \
[  3] = {"close", 1, {INT, 0, 0, 0, 0, 0}, INT}, \
[  4] = {"stat", 2, {STR, STRUCT_STAT, 0, 0, 0, 0}, INT}, \
[  5] = {"fstat", 2, {INT, STRUCT_STAT, 0, 0, 0, 0}, INT}, \
[  6] = {"lstat", 2, {STR, STRUCT_STAT, 0, 0, 0, 0}, INT}, \
[  7] = {"poll", 3, {STRUCT_POLL, ULONG, INT, 0, 0, 0}, INT}, \
[  8] = {"lseek", 3, {INT, PTR, INT, 0, 0, 0}, PTR}, \
[  9] = {"mmap", 6, {PTR, ULONG, FLAG_PROT, FLAG_MMAP, INT, PTR}, PTR}, \
[ 10] = {"mprotect", 3, {PTR, ULONG, INT, 0, 0, 0}, INT}, \
[ 11] = {"munmap", 2, {PTR, ULONG, 0, 0, 0, 0}, INT}, \
[ 12] = {"brk", 1, {PTR, 0, 0, 0, 0, 0}, PTR}, \
[ 13] = {"rt_sigaction", 4, {INT, STRUCT_SIGACT, STRUCT_SIGACT, 0, 0, 0}, INT}, \
[ 14] = {"rt_sigprocmask", 4, {INT, STRUCT_SIGSET, STRUCT_SIGSET, 0, 0, 0}, INT}, \
[ 15] = {"rt_sigreturn", 0, {0, 0, 0, 0, 0, 0}, INT}, \
[ 16] = {"ioctl", 3, {INT, ULONG, 0, 0, 0, 0}, INT}, \
[ 17] = {"pread64", 4, {INT, PTR, ULONG, PTR, 0, 0}, ULONG}, \
[ 18] = {"pwrite64", 4, {INT, PTR, ULONG, PTR, 0, 0}, ULONG}, \
[ 19] = {"readv", 3, {INT, STRUCT_IOVEC, INT, 0, 0, 0}, ULONG}, \
[ 20] = {"writev", 3, {INT, STRUCT_IOVEC, INT, 0, 0, 0}, ULONG}, \
[ 21] = {"access", 2, {STR, INT, 0, 0, 0, 0}, INT}, \
[ 22] = {"pipe", 1, {PIPE, INT, 0, 0, 0, 0}, INT}, \
[ 23] = {"select", 5, {INT, STRUCT_FDSET, STRUCT_FDSET, STRUCT_FDSET, STRUCT_TIMEVAL, 0}, INT}, \
[ 24] = {"sched_yield", 0, {0, 0, 0, 0, 0, 0}, INT}, \
[ 25] = {"mremap", 5, {PTR, ULONG, ULONG, INT, PTR, 0}, PTR}, \
[ 26] = {"msync", 3, {PTR, ULONG, INT, 0, 0, 0}, INT}, \
[ 27] = {"mincore", 3, {PTR, ULONG, STR, 0, 0, 0}, INT}, \
[ 28] = {"madvise", 3, {PTR, ULONG, INT, 0, 0, 0}, INT}, \
[ 29] = {"shmget", 3, {KEY, ULONG, INT, 0, 0, 0}, INT}, \
[ 30] = {"shmat", 3, {INT, PTR, INT, 0, 0, 0}, INT}, \
[ 31] = {"shmctl", 3, {INT, INT, STRUCT_SHMID, 0, 0, 0}, INT}, \
[ 32] = {"dup", 1, {INT, 0, 0, 0, 0, 0}, INT}, \
[ 33] = {"dup2", 2, {INT, INT, 0, 0, 0, 0}, INT}, \
[ 34] = {"pause", 0, {0, 0, 0, 0, 0, 0}, INT}, \
[ 35] = {"nanosleep", 2, {STRUCT_TIMESPEC, STRUCT_TIMESPEC, 0, 0, 0, 0}, INT}, \
[ 36] = {"getitimer", 2, {INT, STRUCT_TIMEVAL, 0, 0, 0, 0}, INT}, \
[ 37] = {"alarm", 1, {ULONG, 0, 0, 0, 0, 0}, ULONG}, \
[ 38] = {"setitimer", 3, {INT, STRUCT_TIMEVAL, STRUCT_TIMEVAL, 0, 0, 0}, INT}, \
[ 39] = {"getpid", 0, {0, 0, 0, 0, 0, 0}, INT}, \
[ 40] = {"sendfile", 4, {INT, INT, PTR, ULONG, 0, 0}, ULONG}, \
[ 41] = {"socket", 3, {INT, INT, INT, 0, 0, 0}, INT}, \
[ 42] = {"connect", 3, {INT, STRUCT_SOCKADDR, ULONG, 0, 0, 0}, INT}, \
[ 43] = {"accept", 3, {INT, STRUCT_SOCKADDR, ULONG, 0, 0, 0}, INT}, \
[ 44] = {"sendto", 6, {INT, PTR, ULONG, INT, STRUCT_SOCKADDR, ULONG}, ULONG}, \
[ 45] = {"recvfrom", 6, {INT, PTR, ULONG, INT, STRUCT_SOCKADDR, ULONG}, ULONG}, \
[ 46] = {"sendmsg", 3, {INT, STRUCT_MSGHDR, INT, 0, 0, 0}, ULONG}, \
[ 47] = {"recvmsg", 3, {INT, STRUCT_MSGHDR, INT, 0, 0, 0}, ULONG}, \
[ 48] = {"shutdown", 2, {INT, INT, 0, 0, 0, 0}, INT}, \
[ 49] = {"bind", 3, {INT, STRUCT_SOCKADDR, ULONG, 0, 0, 0}, INT}, \
[ 50] = {"listen", 2, {INT, INT, 0, 0, 0, 0}, INT}, \
[ 51] = {"getsockname", 3, {INT, STRUCT_SOCKADDR, ULONG, 0, 0, 0}, INT}, \
[ 52] = {"getpeername", 3, {INT, STRUCT_SOCKADDR, ULONG, 0, 0, 0}, INT}, \
[ 53] = {"socketpair", 4, {INT, INT, INT, SV, 0, 0}, INT}, \
[ 54] = {"setsockopt", 5, {INT, INT, INT, PTR, ULONG, 0}, INT}, \
[ 55] = {"getsockopt", 5, {INT, INT, INT, PTR, ULONG, 0}, INT}, \
[ 56] = {"clone", 5, {PTR, PTR, INT, INT, PTR, 0}, INT}, \
[ 57] = {"fork", 0, {0, 0, 0, 0, 0, 0}, INT}, \
[ 58] = {"vfork", 0, {0, 0, 0, 0, 0, 0}, INT}, \
[ 59] = {"execve", 3, {STR, ARGV, ENVP, 0, 0, 0}, INT}, \
[ 60] = {"exit", 1, {INT, 0, 0, 0, 0, 0}, 0}, \
[ 61] = {"wait4", 4, {INT, PTR, INT, STRUCT_RUSAGE, 0, 0}, INT}, \
[ 62] = {"kill", 2, {INT, INT, 0, 0, 0, 0}, INT}, \
[ 63] = {"uname", 1, {STRUCT_UTSNAME, 0, 0, 0, 0, 0}, INT}, \
[ 64] = {"semget", 3, {KEY, INT, INT, 0, 0, 0}, INT}, \
[ 65] = {"semop", 3, {INT, STRUCT_SEMBUF, ULONG, STRUCT_TIMESPEC, 0, 0}, INT}, \
[ 66] = {"semctl", 4, {INT, INT, INT, 0, 0, 0}, INT}, \
[ 67] = {"shmdt", 1, {PTR, 0, 0, 0, 0, 0}, INT}, \
[ 68] = {"msgget", 2, {KEY, INT, 0, 0, 0, 0}, INT}, \
[ 69] = {"msgsnd", 4, {INT, PTR, ULONG, INT, 0, 0}, INT}, \
[ 70] = {"msgrcv", 5, {INT, PTR, ULONG, ULONG, INT, 0}, ULONG}, \
[ 71] = {"msgctl", 3, {INT, INT, STRUCT_MSGID, 0, 0, 0}, INT}, \
[ 72] = {"fcntl", 3, {INT, INT, 0, 0, 0, 0}, INT}, \
[ 73] = {"flock", 2, {INT, INT, 0, 0, 0, 0}, INT}, \
[ 74] = {"fsync", 1, {INT, 0, 0, 0, 0, 0}, INT}, \
[ 75] = {"fdatasync", 1, {INT, 0, 0, 0, 0, 0}, INT}, \
[ 76] = {"truncate", 2, {STR, PTR, 0, 0, 0, 0}, INT}, \
[ 77] = {"ftruncate", 2, {INT, PTR, 0, 0, 0, 0}, INT}, \
[ 78] = {"getdents", 3, {ULONG, STRUCT_LINUX_DIR, ULONG, 0, 0, 0}, ULONG}, \
[ 79] = {"getcwd", 2, {STR, ULONG, 0, 0, 0, 0}, STR}, \
[ 80] = {"chdir", 1, {STR, 0, 0, 0, 0, 0}, INT}, \
[ 81] = {"fchdir", 1, {INT, 0, 0, 0, 0, 0}, INT}, \
[ 82] = {"rename", 2, {STR, STR, 0, 0, 0, 0}, INT}, \
[ 83] = {"mkdir", 2, {STR, MODE, 0, 0, 0, 0}, INT}, \
[ 84] = {"rmdir", 1, {STR, 0, 0, 0, 0, 0}, INT}, \
[ 85] = {"creat", 2, {STR, MODE, 0, 0, 0, 0}, INT}, \
[ 86] = {"link", 2, {STR, STR, 0, 0, 0, 0}, INT}, \
[ 87] = {"unlink", 1, {STR, 0, 0, 0, 0, 0}, INT}, \
[ 88] = {"symlink", 2, {STR, STR, 0, 0, 0, 0}, INT}, \
[ 89] = {"readlink", 3, {STR, STR, ULONG, 0, 0, 0}, ULONG}, \
[ 90] = {"chmod", 2, {STR, MODE, 0, 0, 0, 0}, INT}, \
[ 91] = {"fchmod", 2, {INT, MODE, 0, 0, 0, 0}, 0}, \
[ 92] = {"chown", 3, {STR, INT, INT, 0, 0, 0}, INT}, \
[ 93] = {"fchown", 3, {INT, INT, INT, 0, 0, 0}, INT}, \
[ 94] = {"lchown", 3, {STR, INT, INT, 0, 0, 0}, INT}, \
[ 95] = {"umask", 1, {MODE, 0, 0, 0, 0, 0}, MODE}, \
[ 96] = {"gettimeofday", 2, {STRUCT_TIMEVAL, STRUCT_TIMEZONE, 0, 0, 0, 0}, INT}, \
[ 97] = {"getrlimit", 2, {INT, STRUCT_RLIMIT, 0, 0, 0, 0}, INT}, \
[ 98] = {"getrusage", 2, {INT, STRUCT_RUSAGE, 0, 0, 0, 0}, INT}, \
[ 99] = {"sysinfo", 1, {STRUCT_SYSINFO, 0, 0, 0, 0, 0}, INT}, \
[100] = {"times", 1, {STRUCT_TMS, 0, 0, 0, 0, 0}, CLOCK}, \
[101] = {"ptrace", 4, {PTRACE, INT, PTR, PTR, 0, 0}, ULONG}, \
[102] = {"getuid", 0, {0, 0, 0, 0, 0, 0}, INT}, \
[103] = {"syslog", 3, {INT, STR, 0, 0, 0, 0}, INT}, \
[104] = {"getgid", 0, {0, 0, 0, 0, 0, 0}, INT}, \
[105] = {"setuid", 1, {INT, 0, 0, 0, 0, 0}, INT}, \
[106] = {"setgid", 1, {INT, 0, 0, 0, 0, 0}, INT}, \
[107] = {"geteuid", 0, {0, 0, 0, 0, 0, 0}, INT}, \
[108] = {"getegid", 0, {0, 0, 0, 0, 0, 0}, INT}, \
[109] = {"setpgid", 2, {INT, INT, 0, 0, 0, 0}, INT}, \
[110] = {"getppid", 0, {0, 0, 0, 0, 0, 0}, INT}, \
[111] = {"getpgrp", 0, {0, 0, 0, 0, 0, 0}, INT}, \
[112] = {"setsid", 0, {0, 0, 0, 0, 0, 0}, INT}, \
[113] = {"setreuid", 2, {INT, INT, 0, 0, 0, 0}, INT}, \
[114] = {"setregid", 2, {INT, INT, 0, 0, 0, 0}, INT}, \
[115] = {"getgroups", 2, {INT, ID_T, 0, 0, 0, 0}, INT}, \
[116] = {"setgroups", 2, {INT, ID_T, 0, 0, 0, 0}, INT}, \
[117] = {"setresuid", 3, {INT, INT, INT, 0, 0, 0}, INT}, \
[118] = {"getresuid", 3, {PTR, PTR, PTR, 0, 0, 0}, INT}, \
[119] = {"setresgid", 3, {INT, INT, INT, 0, 0, 0}, INT}, \
[120] = {"getresgid", 3, {PTR, PTR, PTR, 0, 0, 0}, INT}, \
[121] = {"getpgid", 1, {INT, 0, 0, 0, 0, 0}, INT}, \
[122] = {"setfsuid", 1, {INT, 0, 0, 0, 0, 0}, INT}, \
[123] = {"setfsgid", 1, {INT, 0, 0, 0, 0, 0}, INT}, \
[124] = {"getsid", 1, {INT, 0, 0, 0, 0, 0}, INT}, \
[125] = {"capget", 2, {PTR, PTR, 0, 0, 0, 0}, INT}, \
[126] = {"capset", 2, {PTR, PTR, 0, 0, 0, 0}, INT}, \
[127] = {"rt_sigpending", 2, {STRUCT_SIGSET, 0, 0, 0, 0, 0}, INT}, \
[128] = {"rt_sigtimedwait", 4, {STRUCT_SIGSET, STRUCT_SIGINF, 0, 0, 0, 0}, INT}, \
[129] = {"rt_sigqueueinfo", 3, {INT, INT, STRUCT_SIGINF, 0, 0, 0}, INT}, \
[130] = {"rt_sigsuspend", 2, {STRUCT_SIGSET, 0, 0, 0, 0, 0}, INT}, \
[131] = {"sigaltstack", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[132] = {"utime", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[133] = {"mknod", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[134] = {"uselib", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[135] = {"personality", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[136] = {"ustat", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[137] = {"statfs", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[138] = {"fstatfs", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[139] = {"sysfs", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[140] = {"getpriority", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[141] = {"setpriority", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[142] = {"sched_setparam", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[143] = {"sched_getparam", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[144] = {"sched_setscheduler", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[145] = {"sched_getscheduler", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[146] = {"sched_get_priority_max", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[147] = {"sched_get_priority_min", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[148] = {"sched_rr_get_interval", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[149] = {"mlock", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[150] = {"munlock", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[151] = {"mlockall", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[152] = {"munlockall", 0, {0, 0, 0, 0, 0, 0}, 0}, \
[153] = {"vhangup", 0, {0, 0, 0, 0, 0, 0}, 0}, \
[154] = {"modify_ldt", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[155] = {"pivot_root", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[156] = {"_sysctl", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[157] = {"prctl", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[158] = {"arch_prctl", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[159] = {"adjtimex", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[160] = {"setrlimit", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[161] = {"chroot", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[162] = {"sync", 0, {0, 0, 0, 0, 0, 0}, 0}, \
[163] = {"acct", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[164] = {"settimeofday", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[165] = {"mount", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[166] = {"umount2", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[167] = {"swapon", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[168] = {"swapoff", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[169] = {"reboot", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[170] = {"sethostname", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[171] = {"setdomainname", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[172] = {"iopl", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[173] = {"ioperm", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[174] = {"create_module", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[175] = {"init_module", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[176] = {"delete_module", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[177] = {"get_kernel_syms", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[178] = {"query_module", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[179] = {"quotactl", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[180] = {"nfsservctl", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[181] = {"getpmsg", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[182] = {"putpmsg", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[183] = {"afs_syscall", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[184] = {"tuxcall", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[185] = {"security", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[186] = {"gettid", 0, {0, 0, 0, 0, 0, 0}, 0}, \
[187] = {"readahead", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[188] = {"setxattr", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[189] = {"lsetxattr", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[190] = {"fsetxattr", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[191] = {"getxattr", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[192] = {"lgetxattr", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[193] = {"fgetxattr", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[194] = {"listxattr", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[195] = {"llistxattr", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[196] = {"flistxattr", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[197] = {"removexattr", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[198] = {"lremovexattr", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[199] = {"fremovexattr", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[200] = {"tkill", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[201] = {"time", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[202] = {"futex", 6, {0, 0, 0, 0, 0, 0}, 0}, \
[203] = {"sched_setaffinity", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[204] = {"sched_getaffinity", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[205] = {"set_thread_area", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[206] = {"io_setup", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[207] = {"io_destroy", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[208] = {"io_getevents", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[209] = {"io_submit", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[210] = {"io_cancel", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[211] = {"get_thread_area", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[212] = {"lookup_dcookie", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[213] = {"epoll_create", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[214] = {"epoll_ctl_old", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[215] = {"epoll_wait_old", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[216] = {"remap_file_pages", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[217] = {"getdents64", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[218] = {"set_tid_address", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[219] = {"restart_syscall", 0, {0, 0, 0, 0, 0, 0}, 0}, \
[220] = {"semtimedop", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[221] = {"fadvise64", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[222] = {"timer_create", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[223] = {"timer_settime", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[224] = {"timer_gettime", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[225] = {"timer_getoverrun", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[226] = {"timer_delete", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[227] = {"clock_settime", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[228] = {"clock_gettime", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[229] = {"clock_getres", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[230] = {"clock_nanosleep", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[231] = {"exit_group", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[232] = {"epoll_wait", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[233] = {"epoll_ctl", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[234] = {"tgkill", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[235] = {"utimes", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[236] = {"vserver", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[237] = {"mbind", 6, {0, 0, 0, 0, 0, 0}, 0}, \
[238] = {"set_mempolicy", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[239] = {"get_mempolicy", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[240] = {"mq_open", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[241] = {"mq_unlink", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[242] = {"mq_timedsend", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[243] = {"mq_timedreceive", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[244] = {"mq_notify", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[245] = {"mq_getsetattr", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[246] = {"kexec_load", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[247] = {"waitid", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[248] = {"add_key", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[249] = {"request_key", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[250] = {"keyctl", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[251] = {"ioprio_set", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[252] = {"ioprio_get", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[253] = {"inotify_init", 0, {0, 0, 0, 0, 0, 0}, 0}, \
[254] = {"inotify_add_watch", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[255] = {"inotify_rm_watch", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[256] = {"migrate_pages", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[257] = {"openat", 4, {INT, STR, INT, MODE, 0, 0}, INT}, \
[258] = {"mkdirat", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[259] = {"mknodat", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[260] = {"fchownat", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[261] = {"futimesat", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[262] = {"newfstatat", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[263] = {"unlinkat", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[264] = {"renameat", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[265] = {"linkat", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[266] = {"symlinkat", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[267] = {"readlinkat", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[268] = {"fchmodat", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[269] = {"faccessat", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[270] = {"pselect6", 6, {0, 0, 0, 0, 0, 0}, 0}, \
[271] = {"ppoll", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[272] = {"unshare", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[273] = {"set_robust_list", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[274] = {"get_robust_list", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[275] = {"splice", 6, {0, 0, 0, 0, 0, 0}, 0}, \
[276] = {"tee", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[277] = {"sync_file_range", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[278] = {"vmsplice", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[279] = {"move_pages", 6, {0, 0, 0, 0, 0, 0}, 0}, \
[280] = {"utimensat", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[281] = {"epoll_pwait", 6, {0, 0, 0, 0, 0, 0}, 0}, \
[282] = {"signalfd", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[283] = {"timerfd_create", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[284] = {"eventfd", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[285] = {"fallocate", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[286] = {"timerfd_settime", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[287] = {"timerfd_gettime", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[288] = {"accept4", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[289] = {"signalfd4", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[290] = {"eventfd2", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[291] = {"epoll_create1", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[292] = {"dup3", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[293] = {"pipe2", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[294] = {"inotify_init1", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[295] = {"preadv", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[296] = {"pwritev", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[297] = {"rt_tgsigqueueinfo", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[298] = {"perf_event_open", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[299] = {"recvmmsg", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[300] = {"fanotify_init", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[301] = {"fanotify_mark", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[302] = {"prlimit64", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[303] = {"name_to_handle_at", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[304] = {"open_by_handle_at", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[305] = {"clock_adjtime", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[306] = {"syncfs", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[307] = {"sendmmsg", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[308] = {"setns", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[309] = {"getcpu", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[310] = {"process_vm_readv", 6, {0, 0, 0, 0, 0, 0}, 0}, \
[311] = {"process_vm_writev", 6, {0, 0, 0, 0, 0, 0}, 0}, \
[312] = {"kcmp", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[313] = {"finit_module", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[314] = {"sched_setattr", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[315] = {"sched_getattr", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[316] = {"renameat2", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[317] = {"seccomp", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[318] = {"getrandom", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[319] = {"memfd_create", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[320] = {"kexec_file_load", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[321] = {"bpf", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[322] = {"execveat", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[323] = {"userfaultfd", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[324] = {"membarrier", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[325] = {"mlock2", 3, {0, 0, 0, 0, 0, 0}, 0}, \
[326] = {"copy_file_range", 6, {0, 0, 0, 0, 0, 0}, 0}, \
[327] = {"preadv2", 6, {0, 0, 0, 0, 0, 0}, 0}, \
[328] = {"pwritev2", 6, {0, 0, 0, 0, 0, 0}, 0}, \
[329] = {"pkey_mprotect", 4, {0, 0, 0, 0, 0, 0}, 0}, \
[330] = {"pkey_alloc", 2, {0, 0, 0, 0, 0, 0}, 0}, \
[331] = {"pkey_free", 1, {0, 0, 0, 0, 0, 0}, 0}, \
[332] = {"statx", 5, {0, 0, 0, 0, 0, 0}, 0}, \
[333] = {"io_pgetevents", 6, {0, 0, 0, 0, 0, 0}, 0}, \
[334] = {"rseq", 4, {0, 0, 0, 0, 0, 0}, 0} \
}

#endif
