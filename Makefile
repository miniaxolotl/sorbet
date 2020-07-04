#	Last Edit: 2020-05-24
#	Author: Elias Mawa
#
# MAKE TARGETS
# ------------
# the following targets are available :
#	$ make				link and compile
#   $ make demo			build the snake demo
#   $ make run_demo		run the snake demo
#   $ make clean		remove build artifacts


###################################### config

OBJ_NAME = libsorbet

SRC_DIR = src
BUILD_DIR = bin
OBJ_DIR = obj

LIB_DIR = lib
INCLUDE_DIR = include/sorbet

COMPILER_FLAGS = -Wall -Wextra -Werror -O2
LINKER_FLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -llua5.3
CXX = gcc

###################################### targets

SRC = $(wildcard $(SRC_DIR)/*.c)
SRC += $(wildcard $(SRC_DIR)/*/*.c)
SRC += $(wildcard $(SRC_DIR)/*/*/*.c)
SRC += $(wildcard $(SRC_DIR)/*/*/*/*.c)

OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/static/%.o, $(SRC))

###################################### compile scripts

all : $(OBJS)
	mkdir -p $(LIB_DIR)
	ar -rcs $(LIB_DIR)/libsorbet.a $(OBJS)
	mkdir -p $(INCLUDE_DIR)
	cp -r src/* $(INCLUDE_DIR)

###################################### compile helpers

$(OBJ_DIR)/static/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CXX) $(COMPILER_FLAGS) -fPIC -c $< -o $@

###################################### demo

DEMO_SRC = demo/main.c
DEMO_BIN = $(BUILD_DIR)/snake_demo

demo : all
	mkdir -p $(BUILD_DIR)
	$(CXX) $(COMPILER_FLAGS) $(DEMO_SRC) -I. -L$(LIB_DIR) -lsorbet $(LINKER_FLAGS) -o $(DEMO_BIN)

run_demo : demo
	./$(DEMO_BIN)

###################################### utility scripts

clean :
	rm -rf $(LIB_DIR) $(BUILD_DIR) include $(OBJ_DIR) 
