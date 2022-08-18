#include "ft_strace.h"

void			print_siginfo(siginfo_t *si)
{
	fprintf(stderr, "{si_signo=%d, si_code=%d, si_addr=%p}", si->si_signo, si->si_code, si->si_addr);
}

void			print_argv(char **argv)
{
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

void			print_string(pid_t pid, va_list ap)
{
	ssize_t			nread;
	struct iovec	local[1];
	struct iovec	remote[1];

	local[0].iov_base = calloc(BUFFER_SIZE, sizeof(char));
	if (!local[0].iov_base)
		return ;
	local[0].iov_len = BUFFER_SIZE;
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
		{
			free(local[0].iov_base);
			return ;
		}
		if (len > 48)
			fprintf(stderr, "\"%.32s\"...", escaped);
		else
			fprintf(stderr, "\"%s\"", escaped);
		free(escaped);
	}
	free(local[0].iov_base);
}

void			print_ptr(void *ptr)
{
	if (!ptr)
		fprintf(stderr, "NULL", ptr);
	else
		fprintf(stderr, "%#lx", ptr);
}
