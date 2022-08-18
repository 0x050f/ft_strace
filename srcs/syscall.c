#include "ft_strace.h"

const syscall_t			x86_64_syscall[] = X86_64_SYSCALL;
const syscall_t			i386_syscall[] = I386_SYSCALL;
extern const char		*sys_signame[];
extern int				n_envp;

char			*escape(uint8_t *buffer, size_t size)
{
	int			l;
	char		*dest;

	dest = calloc(size * 4 + 1, sizeof(char));
	if (!dest)
		return (NULL);
	l = 0;
	for (size_t i = 0; i < size; i++) {
		if (buffer[i] == '\n')
			l += sprintf(dest + l, "\\n");
		else if (buffer[i] == '\t')
			l += sprintf(dest + l, "\\t");
		else if (buffer[i] == '\r')
			l += sprintf(dest + l, "\\r");
		else if (buffer[i] == '\v')
			l += sprintf(dest + l, "\\v");
		else if (buffer[i] == '\f')
			l += sprintf(dest + l, "\\f");
		else if (buffer[i] < 32 || buffer[i] > 126)
			l += sprintf(dest + l, "\\%d", buffer[i]);
		else
			dest[l++] = buffer[i];
	}
	return (dest);
}

void			print_syscall(pid_t pid, syscall_t syscall, int argc, ...)
{
	struct iovec	local[1];
	struct iovec	remote[1];
	ssize_t			nread;
	va_list			ap;

	local[0].iov_base = malloc(BUFFER_SIZE);
	if (!local[0].iov_base)
		return ;
	local[0].iov_len = BUFFER_SIZE;
	fprintf(stderr, "%s(", syscall.name);
	va_start(ap, argc);
	for (int i = 0; i < argc; i++)
	{
		memset(local[0].iov_base, 0, BUFFER_SIZE);
		if (syscall.type_args[i] == INT)
			fprintf(stderr, "%d", va_arg(ap, int));
		else if (syscall.type_args[i] == ULONG)
			fprintf(stderr, "%ld", va_arg(ap, unsigned long));
		else if (syscall.type_args[i] == ARGV)
		{
			char **argv = va_arg(ap, char **);

			int i = 0;
			fprintf(stderr, "[");
			while (argv[i])
			{
				if (i != 0)
					fprintf(stderr, ", ", *argv);
				fprintf(stderr, "\"%s\"", argv[i]);
				i++;
			}
			fprintf(stderr, "]");
		}
		else if (syscall.type_args[i] == ENVP)
			fprintf(stderr, "%p /* %d vars */", va_arg(ap, void *), n_envp);
		else if (syscall.type_args[i] == STR)
		{
			remote[0].iov_base = (void *)va_arg(ap, void *);
			remote[0].iov_len = BUFFER_SIZE;
			nread = process_vm_readv(pid, local, 1, remote, 1, 0);
			if (nread < 0)
				fprintf(stderr, "%#lx", remote[0].iov_base);
			else
			{
				int len = memchr(local[0].iov_base, 0, BUFFER_SIZE) - local[0].iov_base;
				char *escaped = escape(local[0].iov_base, len);
				if (!escaped)
					break ;
				if (len > 48)
					fprintf(stderr, "\"%.32s\"...", escaped);
				else
					fprintf(stderr, "\"%s\"", escaped);
				free(escaped);
			}
		}
		else if (syscall.type_args[i] == PTR)
		{
			unsigned long ptr = va_arg(ap, unsigned long);
			if (!ptr)
				fprintf(stderr, "NULL", ptr);
			else
				fprintf(stderr, "%#lx", ptr);
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

void			handle_x86_64_syscall(pid_t pid, syscall_handle_t *handler, struct user_regs_struct *regs)
{
	if (handler->start || !strcmp("execve", x86_64_syscall[regs->orig_rax].name))
	{
		if (regs->rax == (unsigned long) -ENOSYS && handler->print && !handler->result && regs->orig_rax < MAX_X86_64_SYSCALL)
		{
			handler->result = true;
			print_syscall(pid, x86_64_syscall[regs->orig_rax], x86_64_syscall[regs->orig_rax].argc, regs->rdi, regs->rsi, regs->rdx, regs->r10, regs->r8, regs->r9);
		}
		else
		{
			if (handler->print && handler->result)
			{
				handler->result = false;
				if (x86_64_syscall[regs->orig_rax].type_ret == INT)
					fprintf(stderr, " = %d\n", regs->rax);
				else
					fprintf(stderr, " = %#lx\n", regs->rax);
			}
			if (!handler->start && (int)regs->rax < 0) //execve failed
				handler->print = false;
			else if (!handler->start)
				handler->start = true;
		}
	}
	if (handler->mode != MODE_64BITS)
		fprintf(stderr, "[ Process PID=%d runs in 64 bit mode. ]\n", pid);
	handler->mode = MODE_64BITS;
}

void			handle_i386_syscall(pid_t pid, syscall_handle_t *handler, struct i386_user_regs_struct *regs)
{
	if (regs->eax == -ENOSYS && handler->print && !handler->result && regs->orig_eax < MAX_I386_SYSCALL)
	{
		handler->result = true;
		print_syscall(pid, i386_syscall[regs->orig_eax], i386_syscall[regs->orig_eax].argc, regs->ebx, regs->ecx, regs->edx, regs->esi, regs->edi, regs->ebp);
	}
	else
	{
		if (handler->print && handler->result)
		{
			handler->result = false;
			if (i386_syscall[regs->orig_eax].type_ret == INT)
				fprintf(stderr, " = %d\n", regs->eax);
			else
				fprintf(stderr, " = %#lx\n", regs->eax);
		}
		if (!handler->start && (int)regs->eax < 0) //execve failed
			handler->print = false;
		else if (!handler->start)
			handler->start = true;
	}
	if (handler->mode != MODE_32BITS)
		fprintf(stderr, "[ Process PID=%d runs in 32 bit mode. ]\n", pid);
	handler->mode = MODE_32BITS;
}

int				handle_syscall(pid_t pid, syscall_handle_t *handler)
{
	union {
		struct user_regs_struct x86_64_r;
		struct i386_user_regs_struct i386_r;
	}	regs;
	struct iovec x86_io;

	x86_io.iov_base = &regs;
	x86_io.iov_len = sizeof(regs);
	if (ptrace(PTRACE_GETREGSET, pid, NT_PRSTATUS, &x86_io) < 0)
		return (EXIT_FAILURE);
	if (x86_io.iov_len == sizeof(struct i386_user_regs_struct))
		handle_i386_syscall(pid, handler, &regs.i386_r);
	else
		handle_x86_64_syscall(pid, handler, &regs.x86_64_r);
	return (EXIT_SUCCESS);
}

int				get_syscalls(pid_t pid)
{
	siginfo_t					si;
	syscall_handle_t			handler;
	int							signal;
	int							status;

	signal = 0;
	status = 0;
	handler.result = false;
	handler.start = false;
	handler.print = true;
	handler.mode = MODE_64BITS;
	while (1)
	{
		if (ptrace(PTRACE_SYSCALL, pid, NULL, signal) < 0)
			break ;
		if (waitpid(pid, &status, 0) < 0)
			break ;
		if (handler.start && !ptrace(PTRACE_GETSIGINFO, pid, NULL, &si)
&& si.si_signo != SIGTRAP)
		{
			signal = si.si_signo;
			fprintf(stderr, "--- %s ---\n", sys_signame[si.si_signo]);
		}
		else
			signal = 0;
		if (handle_syscall(pid, &handler) == EXIT_FAILURE)
			break ;
	}
	if (handler.start && handler.print && handler.result)
		fprintf(stderr, " = ?\n");
	return (status);
}
