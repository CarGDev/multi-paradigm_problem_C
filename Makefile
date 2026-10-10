.PHONY: all clean
all: statistics

CC = cc
CFLAGS = -Wall -Wextra -g -Iinclude -Isrc -MMD -MP
BUILD = build
SRC = $(shell find src -name '*.c')
OBJ = $(patsubst src/%.c,${BUILD}/%.o,$(SRC))

statistics: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD)/%.o: src/%.c | $(BUILD)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD) statistics

-include $(OBJ:.o=.d)
