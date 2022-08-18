CC			=	gcc
CC_FLAGS	=	-Wall -Wextra -Werror -fno-builtin

DIR_HEADERS	=	./includes/
DIR_SRCS	=	./srcs/
DIR_OBJS	=	./compiled_srcs/

SRCS		=	ft_strace.c \
				exec.c

INCLUDES	=	ft_strace.h \
				x86_64_syscall.h \
				sys_signame.h

OBJS		=	$(SRCS:%.c=$(DIR_OBJS)%.o)
DEPS		=	$(SRCS:%.c=$(DIR_OBJS)%.d)

NAME		=	ft_strace

ifeq ($(BUILD),debug)
	CC_FLAGS		+=	-DDEBUG -g3 -fsanitize=address
	DIR_OBJS		=	./debug-compiled_srcs/
	NAME			=	debug-ft_strace
endif

all:		$(NAME)

$(NAME):	$(OBJS) $(addprefix $(DIR_HEADERS), $(INCLUDES))
			$(CC) $(CC_FLAGS) -I $(DIR_HEADERS) $(OBJS) -o $(NAME)

$(OBJS):	| $(DIR_OBJS)

$(DIR_OBJS)%.o:		$(DIR_SRCS)%.c Makefile
					$(CC) $(CC_FLAGS) -MMD -MP -I $(DIR_HEADERS) -c $< -o $@
-include			$(DEPS)

$(DIR_OBJS):
				mkdir -p $(DIR_OBJS)

clean:
				rm -rf $(DIR_OBJS)

fclean:			clean
				rm -rf $(NAME)

re:				fclean
				$(MAKE) --no-print-directory

.PHONY:			all clean fclean re
