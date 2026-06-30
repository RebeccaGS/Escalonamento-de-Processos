TARGET = escalonador

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I.
DEPFLAGS = -MMD -MP

SRC = main.c IO.c processos.c prontos.c saida.c

OBJ_DIR = obj
OBJ = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC))
DEPS = $(OBJ:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c -o $@ $<

clean:
	rm -rf $(TARGET) $(OBJ_DIR)

-include $(DEPS)

.PHONY: all clean
