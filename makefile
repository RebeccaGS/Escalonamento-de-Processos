TARGET = escalonador

CC = gcc
CFLAGS = -Wall

SRC = main.c

OBJ_DIR = obj
OBJ = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(TARGET) $(OBJ_DIR)

.PHONY: all clean
