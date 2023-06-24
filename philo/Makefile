
SRCS = $(shell find . -type f -name "*.c")
OBJS = $(SRCS:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = philo

all: ${NAME}

${NAME}: ${OBJS} ${SRCS}
	${CC} ${CFLAGS} -o $@ ${OBJS}

clean:
	rm -f ${OBJS}

fclean: clean
	rm -f ${NAME}

re: fclean all

.PHONY: all re clean fclean