#include "ft_strace.h"

#include <sys/uio.h>

static char				*prg_name;
const x86_64_syscall_t	x86_64_syscall[] = X86_64_SYSCALL;
const char				*sys_signame[] = SYS_SIGNAME;

void			print_syscall(pid_t pid, x86_64_syscall_t syscall, int argc, ...)
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
				{
					fprintf(stderr, "\"%.32s\"", local[0].iov_base);
					fprintf(stderr, "...");
				}
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
	struct user_regs_struct		regs;
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
		if (ptrace(PTRACE_GETREGS, pid, 0, &regs) < 0)
			break ;
		if (start || !strcmp("execve", x86_64_syscall[regs.orig_rax].name))
		{
			if (regs.rax == (unsigned long) -ENOSYS && print)
			{
				result = 1;
				print_syscall(pid, x86_64_syscall[regs.orig_rax], x86_64_syscall[regs.orig_rax].argc, regs.rdi, regs.rsi, regs.rdx, regs.r10, regs.r8, regs.r9);
			}
			else
			{
				if (print && result == 1)
				{
					result = 0;
					if (x86_64_syscall[regs.orig_rax].type_ret == INT)
						fprintf(stderr, " = %d\n", regs.rax);
					else
						fprintf(stderr, " = %#lx\n", regs.rax);
				}
				if (!start && (int)regs.rax < 0) //execve failed
					print = false;
				else if (!start)
					start = true;
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
