#include "ft_strace.h"

static char		*prg_name;
const x86_64_syscall_t x86_64_syscall[] = X86_64_SYSCALL;
const char *sys_signame[] = SYS_SIGNAME;

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

void			get_syscalls(pid_t pid)
{
	struct user_regs_struct		regs;
	siginfo_t					si;

	while (1)
	{
		ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
		if (waitid(P_PID, pid, &si, WSTOPPED) || si.si_code != CLD_TRAPPED)
			break ;
		ptrace(PTRACE_GETREGS, pid, 0, &regs);
		if (regs.rax == (unsigned long) -ENOSYS)
			print_syscall(x86_64_syscall[regs.orig_rax].name, x86_64_syscall[regs.orig_rax].argc, regs.rdi, regs.rsi, regs.rdx, regs.r10, regs.r8, regs.r9);
		else
			fprintf(stderr, " = %d\n", regs.rax);
	}
	fprintf(stderr, " = ?\n");
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
//		if (kill(getpid(), SIGSTOP))
//			fprintf(stderr, "%s: kill: %s\n", prg_name, strerror(errno));
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
	if (ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD) < 0)
		fprintf(stderr, "%s: ptrace: %s\n", prg_name, strerror(errno));
	get_syscalls(pid);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		fprintf(stderr, "+++ exited by %s +++\n", sys_signame[WTERMSIG(status)]);
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
