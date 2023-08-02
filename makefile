CC = gcc
CFLAGS = -Wall -ansi -pedantic
SRCS = assembler.c parser.c pre_assembler.c macro.c my_string.c instructions.c directives.c first_pass.c symbol.c registers.c
OBJS = $(SRCS:.c=.o)
HEADERS = parser.h pre_assembler.h macro.h my_string.h instructions.h directives.h first_pass.h symbol.h registers.h globals.h
TARGET = assembler

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
