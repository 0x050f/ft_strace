#include "ft_strace.h"

static char				*prg_name;
const x86_64_syscall_t	x86_64_syscall[] = X86_64_SYSCALL;
const i386_syscall_t	i386_syscall[] = I386_SYSCALL;
const char				*sys_signame[] = SYS_SIGNAME;

void			print_i386_syscall(pid_t pid, i386_syscall_t syscall, int argc, ...)
{
	struct iovec	local[1];
	struct iovec	remote[1];
	ssize_t			nread;
	va_list			ap;

	local[0].iov_base = malloc(4096);
	local[0].iov_len = 4096;
	fprintf(stderr, "%s(", syscall.name);
	va_start(ap, argc);
	for (int i = 0; i < argc; i++)
	{
		memset(local[0].iov_base, 0, 4096);
		if (syscall.type_args[i] == INT)
			fprintf(stderr, "%d", va_arg(ap, int));
		else if (syscall.type_args[i] == ULONG)
			fprintf(stderr, "%ld", va_arg(ap, unsigned long));
		else if (syscall.type_args[i] == STR)
		{
			remote[0].iov_base = (void *)va_arg(ap, void *);
			remote[0].iov_len = 4096;
			nread = process_vm_readv(pid, local, 1, remote, 1, 0);
			if (nread < 0)
				fprintf(stderr, "%#lx", remote[0].iov_base);
			else
			{
				if (memchr(local[0].iov_base, 0, 4096) - local[0].iov_base > 48)
					fprintf(stderr, "\"%.32s\"...", local[0].iov_base);
				else
					fprintf(stderr, "\"%s\"", local[0].iov_base);
			}
		}
		else
			fprintf(stderr, "%#lx", va_arg(ap, unsigned long));
		if (i != argc - 1)
			fprintf(stderr, ", ");
	}
	va_end(ap);
	fprintf(stderr, ")");
	free(local[0].iov_base);
}

void			print_x86_64_syscall(pid_t pid, x86_64_syscall_t syscall, int argc, ...)
{
	struct iovec	local[1];
	struct iovec	remote[1];
	ssize_t			nread;
	va_list			ap;

	local[0].iov_base = malloc(4096);
	local[0].iov_len = 4096;
	fprintf(stderr, "%s(", syscall.name);
	va_start(ap, argc);
	for (int i = 0; i < argc; i++)
	{
		memset(local[0].iov_base, 0, 4096);
		if (syscall.type_args[i] == INT)
			fprintf(stderr, "%d", va_arg(ap, int));
		else if (syscall.type_args[i] == ULONG)
			fprintf(stderr, "%ld", va_arg(ap, unsigned long));
		else if (syscall.type_args[i] == STR)
		{
			remote[0].iov_base = (void *)va_arg(ap, void *);
			remote[0].iov_len = 4096;
			nread = process_vm_readv(pid, local, 1, remote, 1, 0);
			if (nread < 0)
				fprintf(stderr, "%#lx", remote[0].iov_base);
			else
			{
				if (memchr(local[0].iov_base, 0, 4096) - local[0].iov_base > 48)
					fprintf(stderr, "\"%.32s\"...", local[0].iov_base);
				else
					fprintf(stderr, "\"%s\"", local[0].iov_base);
			}
		}
		else
			fprintf(stderr, "%#lx", va_arg(ap, unsigned long));
		if (i != argc - 1)
			fprintf(stderr, ", ");
	}
	va_end(ap);
	fprintf(stderr, ")");
	free(local[0].iov_base);
}

int				get_syscalls(pid_t pid)
{
//	struct user_regs_struct		regs;
	siginfo_t					si;
	bool						start;
	bool						print;
	int							result;
	int							status;
	int							signal;

	result = 0;
	signal = 0;
	start = false;
	print = true;
	while (1)
	{
		if (ptrace(PTRACE_SYSCALL, pid, NULL, signal) < 0)
			break ;
		if (waitpid(pid, &status, 0) < 0)
			break ;
		if (start && !ptrace(PTRACE_GETSIGINFO, pid, NULL, &si)
&& si.si_signo != SIGTRAP)
		{
			signal = si.si_signo;
			fprintf(stderr, "--- %s ---\n", sys_signame[si.si_signo]);
		}
		else
			signal = 0;

		union {
			struct user_regs_struct x86_64_r;
			struct i386_user_regs_struct i386_r;
		} regs;

		struct iovec x86_io;
		x86_io.iov_base = &regs;
		x86_io.iov_len = sizeof(regs);

		if (ptrace(PTRACE_GETREGSET, pid, NT_PRSTATUS, &x86_io) < 0)
			break ;
		if (x86_io.iov_len == sizeof(struct i386_user_regs_struct))
		{
			if (regs.i386_r.eax == -ENOSYS && print)
			{
				result = 1;
				print_i386_syscall(pid, i386_syscall[regs.i386_r.orig_eax], i386_syscall[regs.i386_r.orig_eax].argc, regs.i386_r.ebx, regs.i386_r.ecx, regs.i386_r.edx, regs.i386_r.esi, regs.i386_r.edi, regs.i386_r.ebp);
			}
			else
			{
				if (print && result == 1)
				{
					result = 0;
					if (i386_syscall[regs.i386_r.orig_eax].type_ret == INT)
						fprintf(stderr, " = %d\n", regs.i386_r.eax);
					else
						fprintf(stderr, " = %#lx\n", regs.i386_r.eax);
				}
				if (!start && (int)regs.i386_r.eax < 0) //execve failed
					print = false;
				else if (!start)
					start = true;
			}
		}
		else {
			if (start || !strcmp("execve", x86_64_syscall[regs.x86_64_r.orig_rax].name))
			{
				if (regs.x86_64_r.rax == (unsigned long) -ENOSYS && print)
				{
					result = 1;
					print_x86_64_syscall(pid, x86_64_syscall[regs.x86_64_r.orig_rax], x86_64_syscall[regs.x86_64_r.orig_rax].argc, regs.x86_64_r.rdi, regs.x86_64_r.rsi, regs.x86_64_r.rdx, regs.x86_64_r.r10, regs.x86_64_r.r8, regs.x86_64_r.r9);
				}
				else
				{
					if (print && result == 1)
					{
						result = 0;
						if (x86_64_syscall[regs.x86_64_r.orig_rax].type_ret == INT)
							fprintf(stderr, " = %d\n", regs.x86_64_r.rax);
						else
							fprintf(stderr, " = %#lx\n", regs.x86_64_r.rax);
					}
					if (!start && (int)regs.x86_64_r.rax < 0) //execve failed
						print = false;
					else if (!start)
						start = true;
				}
			}
		}
	}
	if (start && print && result == 1)
		fprintf(stderr, " = ?\n");
	return (status);
}

void		block_signals(pid_t pid)
{
	int				status;
	sigset_t		set;

	sigemptyset(&set);
	sigprocmask(SIG_SETMASK, &set, NULL);
	waitpid(pid, &status, 0);
	sigaddset(&set, SIGHUP);
	sigaddset(&set, SIGINT);
	sigaddset(&set, SIGQUIT);
	sigaddset(&set, SIGPIPE);
	sigaddset(&set, SIGTERM);
	sigprocmask(SIG_BLOCK, &set, NULL);
}

int				strace(char *exec, char *argv[], char *envp[])
{
	int				status;
	pid_t			pid;

	pid = fork();
	if (pid < 0)
	{
		fprintf(stderr, "%s: clone: %s\n", prg_name, strerror(errno));
		return (EXIT_FAILURE);
	}
	else if (!pid)
	{
		raise(SIGSTOP);
		execve(exec, argv, envp);
		fprintf(stderr, "%s: execve: %s\n", prg_name, strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (ptrace(PTRACE_SEIZE, pid, NULL, NULL) < 0)
		fprintf(stderr, "%s: ptrace: %s\n", prg_name, strerror(errno));
	if (ptrace(PTRACE_INTERRUPT, pid, NULL, NULL) < 0)
		fprintf(stderr, "%s: ptrace: %s\n", prg_name, strerror(errno));
	block_signals(pid);
	status = get_syscalls(pid);
	if (WIFSIGNALED(status))
	{
		fprintf(stderr, "+++ killed by %s +++\n", sys_signame[WTERMSIG(status)]);
		kill(getpid(), WTERMSIG(status));
	}
	else
		fprintf(stderr, "+++ exited with %d +++\n", WEXITSTATUS(status));
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

	prg_name = NULL;
	exec = NULL;
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
