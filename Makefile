NAME = creed

# CFLAGS += -std=c89 -Wno-gcc-compat -Wno-comment
# CFLAGS += -std=c99
CFLAGS += -std=c11

# uncomment to make warnings into errors
# CFLAGS += -Werror

# uncomment to stop complaints about unused functions
# CFLAGS += -Wno-unused-function

# CFLAGS += -O
CFLAGS += -g
CFLAGS += -Wall -Wextra -Wshadow -Wpedantic

LIBRARIES = -ltermbox

C_EXE = $(NAME)

all: $(C_EXE)

C_SRC_EXE = $(NAME).c
C_OBJ_EXE = $(C_SRC_EXE:.c=.o)

%.o: %.c
	cc $(CFLAGS) -c -o $@ $^

$(C_EXE): %: %.o
	cc $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

clean:
	rm -f *.o
	rm -f $(C_EXE)
