src = $(wildcard *.c)
obj = $(src:.c=.o)

CC = gcc
FLAGS = -fdiagnostics-color -pedantic
LDFLAGS = -lm
WFLAGS = -Werror -Wall -Wextra -fmax-errors=5 \
		 -Werror=vla -Wno-error=unused-parameter \
		 -Wno-error=parentheses -Wno-parentheses \
		 -Werror=unused-value
NAME = cgol

$(NAME): $(obj)
	$(CC) -o $@ $^ $(FLAGS) $(LDFLAGS) $(WFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) $(NAME)
