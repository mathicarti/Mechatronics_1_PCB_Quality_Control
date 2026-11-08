CC=gcc
CFLAGS=-g -Wall -Wextra -Iinclude
LIBS=-lm
TARGET=pcb_check

SRC_DIR = src
OBJ_DIR = build
INC_DIR = include
# you may need to put your extra files here
_DEPS = types.h bitmap.h connection.h matching.h template.h modes.h board.h
_OBJS = main.o bitmap.o connection.o matching.o template.o modes.o board.o

DEPS = $(patsubst %,$(INC_DIR)/%,$(_DEPS))
OBJS = $(patsubst %,$(OBJ_DIR)/%,$(_OBJS))


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)
