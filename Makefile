C_FLAGS := -Wall -Wextra -Werror
CC := gcc -g
RM := rm
LINKFLAGS := -Llibft -lft
LINKFLAGS_TEST := -lcheck -lm -lpthread -lrt -lcheck_pic -lsubunit

.PHONY: $(TARGET)
.PHONY: $(TARGET_TEST)
.PHONY: clean

VPATH := ./src/ ./obj/ ./include/

# Path for .c , .h and .o Files
SRC_PATH := ./src/
OBJ_PATH := ./obj/
INC_PATH :=

# Executable Name
TARGET := serveur
TARGET_TEST := tests/tests

## Files to compile
# Files common to every target
OBJ_ALL1 :=\
	app.o\
	client.o\
	error.o\
	fd_repository.o\
	loop.o\
	server.o\
	circular_buffer.o\
	message.o\
	command.o\
	commands/nick.o\
	commands/user.o\
	commands/list.o\
	hash_table/ht_create.o\
	hash_table/ht_delete.o\
	hash_table/ht_destroy.o\
	hash_table/ht_hash.o\
	hash_table/ht_insert.o\
	hash_table/ht_num_elements_saved.o\
	hash_table/ht_search.o

# Files common only to main target
OBJ_TARGET1 :=\
	main.o

# Files common only to test target
OBJ_TARGET_TEST1 :=\
	test.o

OBJ_ALL := $(patsubst %,$(OBJ_PATH)%,$(OBJ_ALL1))
OBJ_TARGET := $(patsubst %,$(OBJ_PATH)%,$(OBJ_TARGET1))
OBJ_TARGET_TEST := $(patsubst %,$(OBJ_PATH)%,$(OBJ_TARGET_TEST1))

all: $(TARGET) $(TARGET_TEST)

# Build .o first
$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p obj obj/commands obj/hash_table
	@echo [CC] $<
	@$(CC) $(C_FLAGS) -o $@ -c $< $(INC_PATH)

# Build final Binary
$(TARGET): $(OBJ_ALL) $(OBJ_TARGET)
	@echo [INFO] Creating Binary Executable [$(TARGET)]
	@$(CC) -o $@ $^ $(LINKFLAGS)

# Build final test Binary
$(TARGET_TEST): $(OBJ_ALL) $(OBJ_TARGET_TEST)
	@echo [INFO] Creating Test Binary Executable [$(TARGET_TEST)]
	@mkdir -p tests
	@$(CC) -o $@ $^ $(LINKFLAGS) $(LINKFLAGS_TEST)

# Clean all the object files and the binary
clean:
	@echo "[Cleaning objects]"
	@$(RM) -rfv $(OBJ_PATH)

fclean:
	@echo "[Cleaning objects and binary]"
	@$(RM) -rfv $(OBJ_PATH)
	@$(RM) -rfv $(TARGET)
	@$(RM) -rfv $(TARGET_TEST)

re: fclean all