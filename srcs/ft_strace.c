#include "ft_strace.h"

static char				*prg_name;
const x86_64_syscall_t	x86_64_syscall[] = X86_64_SYSCALL;
const char				*sys_signame[] = SYS_SIGNAME;

void			print_syscall(char *name, int argc, ...)
{
	va_list		ap;

	fprintf(stderr, "%s(", name);
	va_start(ap, argc);
	for (int i = 0; i < argc; i++)
	{
		fprintf(stderr, "%#lx", va_arg(ap, unsigned long));
		if (i != argc - 1)
			fprintf(stderr, ", ");
	}
	va_end(ap);
	fprintf(stderr, ")");
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
				print_syscall(x86_64_syscall[regs.orig_rax].name, x86_64_syscall[regs.orig_rax].argc, regs.rdi, regs.rsi, regs.rdx, regs.r10, regs.r8, regs.r9);
			}
			else
			{
				if (print && result == 1)
				{
					result = 0;
					fprintf(stderr, " = %d\n", regs.rax);
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

void		block_signals()
{
	sigset_t		set;

	sigaddset(&set, SIGHUP);
	sigaddset(&set, SIGINT);
	sigaddset(&set, SIGQUIT);
	sigaddset(&set, SIGPIPE);
	sigaddset(&set, SIGTERM);
	sigprocmask(SIG_BLOCK, &set, NULL);
}

int				strace(char *exec, char *argv[], char *envp[])
{
	sigset_t		set;
	int				status;
	pid_t			pid;

	pid = fork();
	if (pid < 0)
	{
		fprintf(stderr, "%s: fork: %s\n", prg_name, strerror(errno));
		return (EXIT_FAILURE);
	}
	else if (!pid)
	{
		raise(SIGSTOP);
		if (execve(exec, argv, envp) < 0)
		{
			fprintf(stderr, "%s: exec: %s\n", prg_name, strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	if (ptrace(PTRACE_SEIZE, pid, NULL, NULL) < 0)
		fprintf(stderr, "%s: ptrace: %s\n", prg_name, strerror(errno));
	if (ptrace(PTRACE_INTERRUPT, pid, NULL, NULL) < 0)
		fprintf(stderr, "%s: ptrace: %s\n", prg_name, strerror(errno));
	sigemptyset(&set);
	sigprocmask(SIG_SETMASK, &set, NULL);
	waitpid(pid, &status, 0);
	block_signals();
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
