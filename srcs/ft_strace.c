#include "ft_strace.h"

static char		*prg_name;
const x86_64_syscall_t x86_64_syscall[] = X86_64_SYSCALL;

int				strace(char *exec, char *argv[], char *envp[])
{
	struct user_regs_struct regs;
	int				status;
	siginfo_t		si;
	pid_t			pid;

	pid = fork();
	if (pid < 0)
	{
		fprintf(stderr, "%s: fork: %s\n", prg_name, strerror(errno));
		return (EXIT_FAILURE);
	}
	else if (!pid)
	{
		if (execve(exec, argv, envp) < 0)
		{
			fprintf(stderr, "%s: exec: %s\n", prg_name, strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		if (kill(pid, SIGSTOP))
			fprintf(stderr, "%s: kill: %s\n", prg_name, strerror(errno));
		else
		{
			if (ptrace(PTRACE_SEIZE, pid, NULL, NULL) < 0)
				fprintf(stderr, "%s: ptrace: %s\n", prg_name, strerror(errno));
			waitpid(pid, &status, 0);
			while (1)
			{
				ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
				if (waitid(P_PID, pid, &si, WSTOPPED) || si.si_code != CLD_TRAPPED)
					break ;
				ptrace(PTRACE_GETREGS, pid, 0, &regs);
				printf("rax: %ld - %s - %d args\n", regs.orig_rax, x86_64_syscall[regs.orig_rax].name, x86_64_syscall[regs.orig_rax].argc);
				ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
				if (waitid(P_PID, pid, &si, WSTOPPED) || si.si_code != CLD_TRAPPED)
					break ;
			}
			waitpid(pid, &status, 0);
			fprintf(stderr, "+++ exited with %d +++\n", WEXITSTATUS(status));
		}
	}
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
