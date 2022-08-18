#include "ft_strace.h"

static char				*prg_name;
int						n_envp;
const char				*sys_signame[] = SYS_SIGNAME;

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
		free(exec);
		free(prg_name);
		fprintf(stderr, "+++ killed by %s +++\n", sys_signame[WTERMSIG(status)]);
		kill(getpid(), WTERMSIG(status));
	}
	else
		fprintf(stderr, "+++ exited with %d +++\n", WEXITSTATUS(status));
	return (WEXITSTATUS(status));
}

bool			check_args(int argc, char *argv[], char *envp[])
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
	n_envp = 0;
	while (envp[n_envp])
		n_envp++;
	return (true);
}

int				main(int argc, char *argv[], char *envp[])
{
	int		ret;
	char	*exec;

	prg_name = NULL;
	exec = NULL;
	if (!check_args(argc, argv, envp))
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
