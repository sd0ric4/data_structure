# 定义编译器
CC = gcc

# 定义编译选项
CFLAGS = -Wall -g

# 定义目录
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
DIST_DIR = $(BUILD_DIR)/dist

# 定义目标文件
TARGET = $(DIST_DIR)/main

# 定义源文件
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/StringStorage.c

# 定义头文件
HEADERS = $(INCLUDE_DIR)/StringStorage.h

# 定义目标文件
OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.c=.o)))

# 默认目标
all: $(TARGET)

# 链接目标文件生成可执行文件
$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^

# 编译源文件生成目标文件
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# 清理生成的文件
clean:
	rm -rf $(BUILD_DIR)

# 伪目标
.PHONY: all clean