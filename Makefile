first: all

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


C_SRC_MAIN = $(NAME).c
C_SRC_LIB = \
	line.c \

C_OBJ_MAIN = $(C_SRC_MAIN:.c=.o)
C_OBJ_LIB = $(C_SRC_LIB:.c=.o)

C_EXE_MAIN = $(NAME)
C_LIB_MAIN = $(NAME).a

$(C_LIB_MAIN): $(C_OBJ_LIB)
	ar -crs $@ $^

%.o: %.c
	cc $(CFLAGS) -c -o $@ $^

$(C_EXE_MAIN): $(C_OBJ_MAIN) $(C_LIB_MAIN)
	cc $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

clean:
	rm -f *.o
	rm -f $(C_LIB_MAIN)
	rm -f $(C_EXE_MAIN)

all: $(C_EXE_MAIN)
